#pragma once

#include <vector>
#include <Eigen/Dense>
#include "particlestate/ParticlesState.hpp"



class Verlet
{
public:
    Vec nextAccelerationX;
    Vec nextAccelerationY;
    void Initiate(ParticlesState &particles);
    void Update(ParticlesState &particles, double dt);

};
