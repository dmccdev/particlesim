#pragma once

#include "particlestate/ParticlesState.hpp"
#include "integrator/Euler.hpp"
#include "integrator/Verlet.hpp"
#include "integrator/RK4.hpp"



class Pairwise

{
public:

    Euler EulerIntegrator;
    Verlet VerletIntegrator;
    RK4 RK4Integrator;

    ParticlesState particles;
    Pairwise(int particlesCount, int integratorNumber, int particleConfigNumber, double dt);
    void InitialiseParticles();
    void Update();

    void CalculateAccelerations(Vec &positionX, Vec &positionY, Vec &accelerationX, Vec &accelerationY, Vec &mass);
    std::array<double, 2> CalculateAcceleration(double displacementX, double displacementY);

    int particlesCount;
    int integratorNumber;
    int particleConfigNumber;
    double dt;
    double G = 1.0;


};