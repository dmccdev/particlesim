#pragma once
#include "particlestate/ParticlesState.hpp"
#include "integrator/Euler.hpp"
#include "integrator/Verlet.hpp"
#include "integrator/RK4.hpp"
#include "quadtree.hpp"
#include "statistics/statistics.hpp"

class BarnesHut
{
public:
    BarnesHut(int particlesCount, int integratorNumber, int particleConfigNumber, double dt);

    void InitialiseParticles();
    void calculateAccelerations(Vec &positionX, Vec &positionY, Vec &accelerationX, Vec &accelerationY, Vec &mass);

    Euler EulerIntegrator;
    Verlet VerletIntegrator;
    RK4 RK4Integrator;

    ParticlesState particles;
    QuadTree quadtree;
    Statistics Stats;
    
    void Update();

    void calculateAcceleration(int nodeIndex, int particleIndex, Vec &positionX, Vec &positionY, Vec &accelerationX, Vec &accelerationY, Vec &mass);

    int particleCount;
    int integratorNumber;
    int particleConfigNumber;
    int frameCounter = 0;

    double dt;
    double G = 1.0;

    double epsilon = 0.75;
    double epsilonSquared = epsilon*epsilon;

    double theta = 0.5;
    double thetaSquared = theta*theta;

};