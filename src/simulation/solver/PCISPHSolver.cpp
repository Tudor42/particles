#include "PCISPHSolver.h"
#include <cmath>

void PCISPHSolver::init() {
    std::shared_ptr<Simulation> sim = Simulation::getCurrentInstance();
    int n_models = (int) sim->getFluidModels().size();
    m_pred_pos.resize(n_models);
    m_pred_v.resize(n_models);
    m_pressure.resize(n_models);
    pressureAcceleration.resize(n_models);
    m_pcisph_fact.resize(n_models);
    m_pred_density.resize(n_models);

    for (int i = 0; i < n_models; i++)
	{
		std::shared_ptr<FluidModel> fm = sim->getFluidModels()[i];
		m_pred_pos[i].resize(fm->getNumParticles(), glm::vec3(0));
		m_pred_v[i].resize(fm->getNumParticles(), glm::vec3(0));
		m_pressure[i].resize(fm->getNumParticles(), 0.0);
        m_pred_density[i].resize(fm->getNumParticles(), 0.0);
		pressureAcceleration[i].resize(fm->getNumParticles(), glm::vec3(0));
	}

    for (int fluidModelIndex = 0; fluidModelIndex < n_models; fluidModelIndex++)
	{
		std::shared_ptr<FluidModel> model = sim->getFluidModels()[fluidModelIndex];
		m_pcisph_fact[fluidModelIndex] = 0.0;

        double volume = model->particleVolume;

        glm::vec3 sumGradW = glm::vec3(0);
        float sumGradW2 = 0.0;
        float supportRadius = sim->getSupportRadius();
        float diam = 2 * (float) model->getRadius();
        glm::vec3 xi = glm::vec3(0);

        if (sim->isSim2D()) {
            glm::vec3 xj = glm::vec3(-supportRadius, -supportRadius, 0);
            while(xj.x <= supportRadius) {
                while(xj.y <= supportRadius) {
                    if(glm::length(xj-xi) < supportRadius) {
                        glm::vec3 grad = sim->kernelGradientFct(xi-xj, sim->smoothingLength);
                        sumGradW += grad;
                        float length = glm::length(grad);
                        sumGradW2 += length * length;
                    }
                    xj.y += diam;
                }
                xj.x += diam;
                xj.y = -supportRadius;
            }
        } else {
            // TODO calculate for 3D
        }
        float beta = 2.f * (float)(volume * volume);
        m_pcisph_fact[fluidModelIndex] = 1.f / (beta * (sumGradW2 + glm::length(sumGradW) * glm::length(sumGradW)));
    }
}



void PCISPHSolver::step()
{
    std::shared_ptr<Simulation> sim = Simulation::getCurrentInstance();

    //for(int i = 0; i < sim->getFluidModels().size(); ++i) {
    //    sim->computeFluidModelDensities(i);
    //}

    pressureSolve();
}

void PCISPHSolver::pressureSolve() {
    std::shared_ptr<Simulation> sim = Simulation::getCurrentInstance();
    int nFluids = (int) sim->getFluidModels().size();
    for(int i = 0; i < nFluids; ++i) {
        #pragma omp parallel default(shared)
        {  
            #pragma omp for schedule(static) 
            for (int j = 0; j < (int) sim->getFluidModels()[i]->getNumParticles(); ++j) {
                m_pressure[i][j] = 0.f;
                pressureAcceleration[i][j] = glm::vec3(0);
            }
        }
    }

    float averageDensityError = 0;
    int numiterations = 0;
    int minIterations = 15;
    int maxIterations = 100;
    bool chk = false;
    while ((!chk || (numiterations < minIterations)) && (numiterations < maxIterations))
	{
		chk = true;
		for (unsigned int i = 0; i < nFluids; i++)
		{
			std::shared_ptr<FluidModel> model = sim->getFluidModels()[i];
			const float restDensity = model->restDensity;

			averageDensityError = 0.0;
			pressureSolveIteration(i, averageDensityError);

			// Maximal allowed density fluctuation
			const float eta = 0.01f * 0.01f * restDensity;  // maxError is given in percent
			chk = chk && (averageDensityError <= eta);
		}

		numiterations++;
	}
    for(int i = 0; i < nFluids; ++i) {
        #pragma omp parallel default(shared)
        {  
            #pragma omp for schedule(static) 
            for (int j = 0; j < (int) sim->getFluidModels()[i]->getNumParticles(); ++j) {
                glm::vec3 &accel = sim->getFluidModels()[i]->getAcceleration(j);
                accel += pressureAcceleration[i][j] / (float)sim->getFluidModels()[i]->particleMass;
            }
        }
    }

}

void PCISPHSolver::pressureSolveIteration(int fluidIndex, float &avg_density_err) {
    std::shared_ptr<Simulation> sim = Simulation::getCurrentInstance();
	std::shared_ptr<FluidModel> model = sim->getFluidModels()[fluidIndex];

    const int numParticles = model->getNumParticles();
    if (numParticles == 0) {
        return;
    }

    const float restDensity = model->restDensity;
    float dt = sim->dt;
    float dt2 = dt * dt;
    float invDt2 = 1.f/dt2;

    #pragma omp parallel default(shared)
    {
        #pragma omp for schedule(static)
        for (int i = 0; i < (int) model->getNumParticles(); i++)
        {
            const glm::vec3 accel = model->getAcceleration(i) + pressureAcceleration[fluidIndex][i] / (float)sim->getFluidModels()[fluidIndex]->particleMass;
            glm::vec3 &pred_pos = m_pred_pos[fluidIndex][i];
            glm::vec3 &pred_vel = m_pred_v[fluidIndex][i];
            const glm::vec3 &x = model->getPosition(i);
			const glm::vec3 &v = model->getVelocity(i);

            pred_vel = v + dt * accel;
			pred_pos = x + dt * pred_vel;
        }
    }
    auto &fluidPointSet = sim->neighborhoodSearch->point_set(model->point_set_id);

    #pragma omp parallel default(shared)
    {
        #pragma omp for schedule(static)
        for (int i = 0; i < (int) model->getNumParticles(); i++)
        {
            float &density = m_pred_density[fluidIndex][i];

            density = model->particleMass * sim->kernelFct(0, sim->smoothingLength);
            glm::vec3 &xi = m_pred_pos[fluidIndex][i];
            for(const auto &otherFluidModel: sim->getFluidModels()){
                for(int j = 0; j < fluidPointSet.n_neighbors(otherFluidModel->point_set_id, i); ++j) {
                    const unsigned int neighborIndex = fluidPointSet.neighbor(otherFluidModel->point_set_id, i, j);
                    glm::vec3 &xj = m_pred_pos[otherFluidModel->point_set_id][neighborIndex];
                    density += otherFluidModel->particleMass * sim->kernelFct(glm::length(xi-xj), sim->smoothingLength);
                }
            }
            const float density_err = (density - restDensity);
            float &pressure = m_pressure[fluidIndex][i];
            pressure += invDt2 * m_pcisph_fact[fluidIndex] * (density - restDensity);
            pressure = std::max(pressure, 0.f);
            //double sig = std::exp(pressure);
            //model->getColor(i) = glm::vec4(sig/(1+sig),sig/(1+sig), sig/(1+sig), 1);
            // fluidModel->getColor(i) = glm::vec4(density*10, density, 1, 1);
            #pragma omp atomic
            avg_density_err += density_err;
        }
    }
    avg_density_err /= numParticles;


   #pragma omp parallel default(shared)
    {
        #pragma omp for schedule(static)
        for (int i = 0; i < (int) model->getNumParticles(); i++)
        {
            glm::vec3 &xi = model->getPosition(i);
            glm::vec3 &ai = pressureAcceleration[fluidIndex][i];
            ai = glm::vec3(0);
            float dpi = m_pressure[fluidIndex][i];
            float pred_densi = m_pred_density[fluidIndex][i];
            for(const auto &otherFluidModel: sim->getFluidModels()){
                for(int j = 0; j < fluidPointSet.n_neighbors(otherFluidModel->point_set_id, i); ++j) {
                    const unsigned int neighborIndex = fluidPointSet.neighbor(otherFluidModel->point_set_id, i, j);
                    glm::vec3 &xj = otherFluidModel->getPosition(neighborIndex);
                    float dpj = m_pressure[otherFluidModel->point_set_id][neighborIndex];
                    float pred_densj = m_pred_density[otherFluidModel->point_set_id][neighborIndex];
                    ai -= (float) (model->particleMass * (dpi/(pred_densi * pred_densi) + dpj / (pred_densj * pred_densj))) * sim->kernelGradientFct(xi - xj, sim->smoothingLength);
                }
            }
        }
    }
}
