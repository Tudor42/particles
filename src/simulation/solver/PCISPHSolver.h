#pragma once

#include "simulation/solver/Solver.h"
#include "simulation/Simulation.h"
#include <vector>
#include <glm/glm.hpp>


class PCISPHSolver : public Solver {
private:
    std::vector<std::vector<glm::vec3>> m_pred_pos;
    std::vector<std::vector<glm::vec3>> m_pred_v;
    std::vector<std::vector<float>> m_pred_density;
    std::vector<std::vector<float>> m_pressure;
    std::vector<std::vector<glm::vec3>> pressureAcceleration;
    std::vector<float> m_pcisph_fact;

    void pressureSolve();
    void pressureSolveIteration(int fluidIndex, float &avg_density_err);
public:
    virtual void step() override;
    virtual void init() override;
};