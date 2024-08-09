#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class FluidModel { 
private:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> velocities;

    std::vector<glm::vec4> colors;
    std::vector<double> densities;

    unsigned int num_particles;
    double radius;
    double restDensity = 1;

public:
    unsigned int point_set_id;

    void addParticle(glm::vec3 position);
    glm::vec3 &getPosition(int particleIndex) { return positions[particleIndex]; }
    glm::vec3 &getVelocitie(int particleIndex) { return velocities[particleIndex]; }
    double &getDensity(int particleIndex) { return densities[particleIndex]; }

    glm::vec4 &getColor(int particleIndex) { return colors[particleIndex]; }
    
    unsigned int getNumParticles() { return num_particles; }
    double getRadius() { return radius; }
    static std::shared_ptr<FluidModel> createSquare(glm::vec2 upperLeft, glm::vec2 lowerRight, double radius);
};