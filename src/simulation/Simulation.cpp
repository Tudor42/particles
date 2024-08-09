#include "Simulation.h"
#include "simulation/models/BoxBoundary.h"
#include <algorithm>


float dt = 1.f/30;
std::shared_ptr<Simulation> Simulation::m_Current;

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
    neighborhoodSearch->find_neighbors();
    for(int i = 0; i < fluidModels.size(); ++i) {
        computeFluidModelDensities(i);
    }

    for(auto &fluidModels : fluidModels){
        for(unsigned int i = 0; i < fluidModels->getNumParticles(); ++i){
            fluidModels->getVelocitie(i) +=  gravity * dt;

            if (m_is2D) {
                
                float tmp = std::clamp(2 - (fluidModels->getPosition(i).x - BoxBoundary::lowerBoundary.x), 0.f, 2.f);
                if (tmp > 0) {
                    fluidModels->getVelocitie(i).x *= -1;
                } 
                tmp = fluidModels->getVelocitie(i).x =  - std::clamp(2 - (BoxBoundary::upperBoundary.x - fluidModels->getPosition(i).x), 0.f, 2.f);
                if (tmp > 0) {
                    fluidModels->getVelocitie(i).x *= -1;
                }
                tmp = std::clamp(2 - (fluidModels->getPosition(i).y - BoxBoundary::lowerBoundary.y), 0.f, 2.f);
                if (tmp > 0) {
                    fluidModels->getVelocitie(i).y *= -1;
                }
                tmp = - std::clamp(2 - (BoxBoundary::upperBoundary.y - fluidModels->getPosition(i).y), 0.f, 2.f);
                if (tmp > 0) {
                    fluidModels->getVelocitie(i).y *= -1;
                }
            }

            fluidModels->getPosition(i) += fluidModels->getVelocitie(i) * dt;
        }
    }


}

void Simulation::handleBoundary() {

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
            density = kernelFct(0, smoothingLength);
            glm::vec3 &xi = fluidModel->getPosition(i);
            for(const auto &otherFluidModel: fluidModels){
                for(int j = 0; j < fluidPointSet.n_neighbors(otherFluidModel->point_set_id, i); ++j) {
                    const unsigned int neighborIndex = fluidPointSet.neighbor(otherFluidModel->point_set_id, i, j);
                    glm::vec3 &xj = otherFluidModel->getPosition(neighborIndex);
                    density += kernelFct(glm::length(xi-xj), smoothingLength);
                }
            }
            // fluidModel->getColor(i) = glm::vec4(density*10, density, 1, 1);
        }
    }
}

