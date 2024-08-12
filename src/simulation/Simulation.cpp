#include "Simulation.h"
#include "simulation/models/BoxBoundary.h"
#include <algorithm>
#include "gui/colormap.h"
#include <math.h>

std::shared_ptr<Simulation> Simulation::m_Current;
void computeNormals();
glm::vec3 gravity = glm::vec3(0, -9.8, 0);

void Simulation::setCurrentInstance(std::shared_ptr<Simulation> simultion)
{
    m_Current = simultion;
}

std::shared_ptr<Simulation> Simulation::getCurrentInstance()
{  
    return m_Current;
}


void Simulation::step()
{
    for(int i = 0; i < fluidModels.size(); ++i) {
        clearAcceleration(i);
    }

    neighborhoodSearch->find_neighbors();
    for(int i = 0; i < fluidModels.size(); ++i) {
        computeFluidModelDensities(i);
    }
    //if (solver) {
    //    solver->step();
    //}

    computeNormals();
    for(int fmi = 0; fmi < fluidModels.size(); ++fmi){
    std::shared_ptr<FluidModel> m_model = fluidModels[fmi];
    #pragma omp parallel default(shared)
	{
		#pragma omp for schedule(static)  
		for (int i = 0; i < (int)m_model->getNumParticles(); i++)
		{
			const glm::vec3 &xi = m_model->getPosition(i);
			const glm::vec3 &ni = m_model->getNormal(i);
			const float &rhoi = m_model->getDensity(i);
			glm::vec3 &ai = m_model->getAcceleration(i);

            auto &fluidPointSet = neighborhoodSearch->point_set(m_model->point_set_id);

            for(const auto &otherFluidModel: fluidModels){
                for(int j = 0; j < fluidPointSet.n_neighbors(otherFluidModel->point_set_id, i); ++j) {
                    const unsigned int neighborIndex = fluidPointSet.neighbor(otherFluidModel->point_set_id, i, j);
			        const float &rhoj = m_model->getDensity(neighborIndex);

                    const float K_ij = static_cast<float>(2.0)*m_model->restDensity / (rhoi + rhoj);

                    glm::vec3 accel = glm::vec3(0);
                    glm::vec3 xj = otherFluidModel->getPosition(neighborIndex);
                    glm::vec3 xixj = (xi - xj);
                    const float length = glm::length(xixj);
                    const float length2 = length * length;
                    if (length2 > 1.0e-9)
                    {
                        xixj = (1.f / sqrt(length2)) * xixj;
                        accel -= 5.f * (float) m_model->particleMass * xixj * (float) kernelFct(glm::length(xi-xj), smoothingLength);
                    }

                    // Curvature
                    const glm::vec3 &nj = otherFluidModel->getNormal(neighborIndex);
                    accel -= 5.f * (ni - nj);

                    ai += K_ij * accel;
                }
            }
			
        }
    }
    }
    for(int fmi = 0; fmi < fluidModels.size(); ++fmi){
    #pragma omp parallel default(shared)
    {
        std::shared_ptr<FluidModel> fluidModel = fluidModels[fmi];
        auto &fluidPointSet = neighborhoodSearch->point_set(fluidModel->point_set_id);

        #pragma omp for schedule(static)
        for (int i = 0; i < (int) fluidModel->getNumParticles(); i++)
        {
            const double &density = fluidModel->getDensity(i);
            const double &pressure = fluidModel->getPressure(i);
            glm::vec3 &acc = fluidModel->getAcceleration(i);
            
            glm::vec3 &xi = fluidModel->getPosition(i);
            for(const auto &otherFluidModel: fluidModels){
                for(int j = 0; j < fluidPointSet.n_neighbors(otherFluidModel->point_set_id, i); ++j) {
                    const unsigned int neighborIndex = fluidPointSet.neighbor(otherFluidModel->point_set_id, i, j);
                    glm::vec3 &xj = otherFluidModel->getPosition(neighborIndex);
                    if(glm::length(xj-xi) < 1e-7f) {
                        continue;
                    }
                    const double &otherdensity = otherFluidModel->getDensity(neighborIndex);
                    const double &otherpressure = otherFluidModel->getPressure(neighborIndex);
                    acc -= (float)(otherFluidModel->particleMass * density * (pressure/(density * density) + otherpressure/(otherdensity * otherdensity))) * kernelGradientFct(xi-xj, smoothingLength);
                }
            }
        }
    }
    }
  
    if (shouldIntegrate) {
        integrate();
    }

    handleBoundary();
}

void Simulation::integrate() {
    for(int i = 0; i < fluidModels.size(); ++i) {
        #pragma omp parallel default(shared)
        {  
            #pragma omp for schedule(static) 
            for (int j = 0; j < (int) fluidModels[i]->getNumParticles(); ++j) {
                glm::vec3 &vel = fluidModels[i]->getVelocity(j);

                vel += fluidModels[i]->getAcceleration(j)*dt;
                fluidModels[i]->getPosition(j) = fluidModels[i]->getPrevPosition(j) + fluidModels[i]->getVelocity(j)*dt;
                const float* color = &colormap_coolwarm[(int)(glm::length(vel)/100*255)][0];
                fluidModels[i]->getColor(j) = glm::vec4(color[0], color[1], color[2], 1);
            }
        }
    }
}

void Simulation::handleBoundary() {
    for(auto &fluidModels : fluidModels){
        for(unsigned int i = 0; i < fluidModels->getNumParticles(); ++i){
            if (m_is2D) {    
                if(fluidModels->getPosition(i).x > BoxBoundary::upperBoundary.x) {
                    fluidModels->getPosition(i).x = BoxBoundary::upperBoundary.x;
                    fluidModels->getVelocity(i).x *= -0.8f; 
                }
                if(fluidModels->getPosition(i).y > BoxBoundary::upperBoundary.y) {
                    fluidModels->getPosition(i).y = BoxBoundary::upperBoundary.y;
                    fluidModels->getVelocity(i).y *= -0.8f; 
                }
                if(fluidModels->getPosition(i).x < BoxBoundary::lowerBoundary.x) {
                    fluidModels->getPosition(i).x = BoxBoundary::lowerBoundary.x;
                    fluidModels->getVelocity(i).x *= -0.8f; 
                }
                if(fluidModels->getPosition(i).y < BoxBoundary::lowerBoundary.y) {
                    fluidModels->getPosition(i).y = BoxBoundary::lowerBoundary.y;
                    fluidModels->getVelocity(i).y *= -0.8f; 
                }
            }
        }
    }
}

void Simulation::computeFluidModelDensities(int fluidId) {
    std::shared_ptr<FluidModel> fluidModel = fluidModels[fluidId];
    auto &fluidPointSet = neighborhoodSearch->point_set(fluidModel->point_set_id);
    #pragma omp parallel default(shared)
    {
        #pragma omp for schedule(static)
        for (int i = 0; i < (int) fluidModel->getNumParticles(); i++)
        {
            double &density = fluidModel->getDensity(i);

            density = fluidModel->particleMass * kernelFct(0, smoothingLength);
            glm::vec3 &xi = fluidModel->getPosition(i);
            for(const auto &otherFluidModel: fluidModels){
                for(int j = 0; j < fluidPointSet.n_neighbors(otherFluidModel->point_set_id, i); ++j) {
                    const unsigned int neighborIndex = fluidPointSet.neighbor(otherFluidModel->point_set_id, i, j);
                    glm::vec3 &xj = otherFluidModel->getPosition(neighborIndex);
                    density += otherFluidModel->particleMass * kernelFct(glm::length(xi-xj), smoothingLength);
                }
            }

/*            glm::vec3 low = BoxBoundary::lowerBoundary;
            glm::vec3 high = BoxBoundary::upperBoundary;

            density += 5*kernelFct((low.x - xi.x) * (low.x - xi.x) , smoothingLength);
            density += 5*kernelFct((low.y - xi.y) * (low.y - xi.y) , smoothingLength);
            density += 5*kernelFct((high.x - xi.x) * (high.x - xi.x) , smoothingLength);
            density += 5*kernelFct((high.y - xi.y) * (high.y - xi.y) , smoothingLength);
*/

            fluidModel->getPressure(i) = std::max(300.*fluidModel->restDensity / 7 *(std::pow(density/fluidModel->restDensity, 7.) - 1), 0.);
            // fluidModel->getColor(i) = glm::vec4(density*10, density, 1, 1);
        }
    }
}

void computeNormals() {
    std::shared_ptr<Simulation> sim = Simulation::getCurrentInstance();


    for(int ii =0; ii <sim->getFluidModels().size();++ii){
    std::shared_ptr<FluidModel> model = sim->getFluidModels()[ii];

    const float supportRadius = sim->getSupportRadius();
    const unsigned int numParticles = model->getNumParticles();
    const unsigned int fluidModelIndex = model->point_set_id;
    auto &fluidPointSet = sim->neighborhoodSearch->point_set(model->point_set_id);

    // Compute normals
    #pragma omp parallel default(shared)
    {
        #pragma omp for schedule(static)  
        for (int i = 0; i < (int)numParticles; i++)
        {
            const glm::vec3 &xi = model->getPosition(i);
            glm::vec3 &ni = model->getNormal(i);
            ni = glm::vec3(0);
            
            ni = supportRadius*ni;
            for(const auto &otherFluidModel: sim->getFluidModels()){
                for(int j = 0; j < fluidPointSet.n_neighbors(otherFluidModel->point_set_id, i); ++j) {
                    const unsigned int neighborIndex = fluidPointSet.neighbor(otherFluidModel->point_set_id, i, j);
                    const float density_j = otherFluidModel->getDensity(neighborIndex);
                    const glm::vec3 &xj = otherFluidModel->getPosition(neighborIndex);

					ni += (float)(model->particleMass / density_j) * sim->kernelGradientFct(xi - xj, sim->smoothingLength);
                }
            }
            ni = supportRadius*ni;
        }
    }
    }
}

void Simulation::clearAcceleration(int fluidId) { 
    std::shared_ptr<FluidModel> fluidModel = fluidModels[fluidId];
    #pragma omp parallel default(shared)
    {  
        #pragma omp for schedule(static) 
        for (int i = 0; i < (int) fluidModel->getNumParticles(); ++i) {
            //fluidModel->getAcceleration(i) = gravity;
            fluidModel->getAcceleration(i) = glm::vec3(0);
            float maxSpeed = 100;
            fluidModel->getVelocity(i) += gravity * dt;
            if (glm::length(fluidModel->getVelocity(i)) > maxSpeed) {
                fluidModel->getVelocity(i) = maxSpeed * glm::normalize(fluidModel->getVelocity(i));
            }
            fluidModel->getPrevPosition(i) = fluidModel->getPosition(i);
            fluidModel->getPosition(i) += fluidModel->getVelocity(i) * 1.f/120.f;
        }
    }
}