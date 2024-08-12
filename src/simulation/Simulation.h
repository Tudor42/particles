#pragma once

#include <vector>
#include "simulation/models/FluidModel.h"
#include "utils/NeighborhoodSearch.h"
#include <glm/gtc/type_ptr.hpp>
#include "simulation/solver/Solver.h"

class Simulation {
public:
    static void setCurrentInstance(std::shared_ptr<Simulation> simultion);
    static std::shared_ptr<Simulation> getCurrentInstance();

    bool isSim2D() { return m_is2D; }

    Simulation(bool is2D = true, float searchRadius = 30) : m_is2D(is2D) {
        neighborhoodSearch = std::make_shared<NeighborhoodSearch>(searchRadius);
        compactSupport = searchRadius;
    }

    void setSearchRadius(double r) { neighborhoodSearch->set_radius((float)r); compactSupport = r; }

    void addFluidModel(std::shared_ptr<FluidModel> fluidModel) {
        numModels++;
        fluidModels.push_back(fluidModel); 
        fluidModel->point_set_id = neighborhoodSearch->add_point_set(glm::value_ptr(fluidModel->getPosition(0)), fluidModel->getNumParticles());
    }
    
    void step();
    const std::vector<std::shared_ptr<FluidModel>>& getFluidModels() { return fluidModels; }

    double (*kernelFct)(double r, double h);
    glm::vec3 (*kernelGradientFct)(glm::vec3 rij, double h);

    double smoothingLength;
    void computeFluidModelDensities(int fluidId);

    void initSolver() { solver->init(); }
    void setSolver(const std::shared_ptr<Solver> &solver) { this->solver = solver; }

    int getNumberModels() { return numModels; }

    float getSupportRadius() { return (float) compactSupport; }

    bool shouldIntegrate = true;
    float dt = 1.f/60;

    std::shared_ptr<NeighborhoodSearch> neighborhoodSearch;

private:
    bool m_is2D;
    int numModels;
    std::vector<std::shared_ptr<FluidModel>> fluidModels;
    std::shared_ptr<Solver> solver;
    void handleBoundary();
    void clearAcceleration(int fluidId);
    void integrate();


    double compactSupport;
    static std::shared_ptr<Simulation> m_Current;
};