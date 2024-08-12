#include "FluidModel.h"

#define DEFAULT_FLUID_COLOR glm::vec4(0, 164./255, 186./255, 1)

void FluidModel::addParticle(glm::vec3 position)
{
    num_particles++;
    positions.push_back(position);
    velocities.push_back(glm::vec3(0));
    accelerations.push_back(glm::vec3(0));
    colors.push_back(DEFAULT_FLUID_COLOR);
    densities.push_back(0);
    pressure.push_back(0);
    prevPositions.push_back(position);
    normals.push_back(glm::vec3(0));
}

std::shared_ptr<FluidModel> FluidModel::createSquare(glm::vec2 upperLeft, glm::vec2 lowerRight, double radius)
{
    std::shared_ptr<FluidModel> fluid = std::make_shared<FluidModel>();
    glm::vec3 currPos = glm::vec3(upperLeft, 0);
    while (currPos.y > lowerRight.y) {
        currPos.x = upperLeft.x;
        while (currPos.x < lowerRight.x) {
            fluid->addParticle(currPos);
            currPos.x += 2.2f * radius;
        }
        currPos.y -= 2.2f * radius ;
    }
    fluid->radius = radius;
    fluid->particleVolume = 4*radius*radius;
    fluid->particleMass = fluid->particleVolume * fluid->restDensity;
    return fluid;
}