#pragma once
#include "ParticlesState.hpp"
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
    void initialiseParticles();
    void Update(double dt);

    int particlesCount;
    int integratorNumber;
    int particleConfigNumber;


};