#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class FluidModel { 
private:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> prevPositions;
    std::vector<glm::vec3> velocities;
    std::vector<glm::vec3> accelerations;
    std::vector<glm::vec3> normals;


    std::vector<glm::vec4> colors;
    std::vector<double> densities;
    std::vector<double> pressure;

    unsigned int num_particles;
    double radius;

public:
    unsigned int point_set_id;
    double restDensity = 1.5;
    double particleMass;
    double particleVolume;

    void addParticle(glm::vec3 position);
    glm::vec3 &getPosition(int particleIndex) { return positions[particleIndex]; }
    glm::vec3 &getPrevPosition(int particleIndex) { return prevPositions[particleIndex]; }

    glm::vec3 &getVelocity(int particleIndex) { return velocities[particleIndex]; }
    glm::vec3 &getAcceleration(int particleIndex) { return accelerations[particleIndex]; }
    glm::vec3 &getNormal(int particleIndex) { return normals[particleIndex]; }

    double &getDensity(int particleIndex) { return densities[particleIndex]; }
    double &getPressure(int particleIndex) { return pressure[particleIndex]; }

    glm::vec4 &getColor(int particleIndex) { return colors[particleIndex]; }

    unsigned int getNumParticles() { return num_particles; }
    double getRadius() { return radius; }
    static std::shared_ptr<FluidModel> createSquare(glm::vec2 upperLeft, glm::vec2 lowerRight, double radius);
};