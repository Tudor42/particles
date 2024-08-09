#pragma once

#include <vector>
#include "simulation/models/FluidModel.h"


class Simulation {
public:
    static bool isSim2D() { return m_is2D; }
    static void addFluidModel(FluidModel *fluidModel) { fluidModels.push_back(fluidModel); }
    static void step();

private:
    static bool m_is2D;
    static std::vector<FluidModel*> fluidModels;
    
};