#pragma once
#include "ParticlesState.hpp"
#include "integrator/Euler.hpp"
#include "integrator/Verlet.hpp"
#include "integrator/RK4.hpp"



class PairwiseAlgorithm

{
public:

    Euler EulerIntegrator;
    Verlet VerletIntegrator;
    RK4 RK4Integrator;

    ParticlesState particles;
    PairwiseAlgorithm(int numberOfParticles, int integratorNum, int particleConfigNumber, double dt);
    void initialiseParticles();
    void Update(double dt);

    int numberOfParticles;
    int integratorNum;
    int particleConfigNumber;


};