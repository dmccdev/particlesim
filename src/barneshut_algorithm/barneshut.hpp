#pragma once
#include "pairwise_algorithm/ParticlesState.hpp"
#include "integrator/Euler.hpp"
#include "integrator/Verlet.hpp"
#include "integrator/RK4.hpp"
#include "quadtree.hpp"

class BarnesHut
{
public:
    BarnesHut(int particlesCount, int integratorNumber, int particleConfigNumber, double dt);

    void InitialiseParticles();

    Euler EulerIntegrator;
    Verlet VerletIntegrator;
    RK4 RK4Integrator;

    ParticlesState particles;
    QuadTree quadtree;
    
    void Update();

    void calculateAcceleration(int nodeIndex, int particleIndex);

    int particleCount;
    int integratorNumber;
    int particleConfigNumber;

    double dt;
    double G = 0.1;

    double epsilon = 0.75;
    double epsilonSquared = epsilon*epsilon;

    double theta = 0.5;
    double thetaSquared = theta*theta;

};