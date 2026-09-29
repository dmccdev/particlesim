#pragma once

#include <Eigen/Dense>
#include "particlestate/ParticlesState.hpp"

using Vec = Eigen::VectorXd;

class Integrator
{
public:
    double G = 1.0;

    std::array<double, 2> CalculateAcceleration(double displacementX, double displacementY);
    void Update(ParticlesState &particles, double dt);
    void CalculateAccelerations(Vec &positionX, Vec &positionY, Vec &accelerationX, Vec &accelerationY, Vec &mass);
};