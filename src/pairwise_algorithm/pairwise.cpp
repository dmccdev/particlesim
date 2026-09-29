#include "pairwise.hpp"
#include <random>
#include "integrator/Euler.hpp"
#include "integrator/Verlet.hpp"
#include "integrator/RK4.hpp"


Pairwise::Pairwise(int particlesCount, int integratorNumber, int particleConfigNumber, double dt):
particlesCount(particlesCount),
integratorNumber(integratorNumber),
particleConfigNumber(particleConfigNumber),
particles(particlesCount),
RK4Integrator(particlesCount),
dt(dt)
{
}

void Pairwise::InitialiseParticles()
{
    switch(particleConfigNumber) //Initalise Particle Configuration
    {
    case 1:
        particles.Galaxy();
        break;
    case 2:
        particles.BinaryGalaxy();
        break;
    case 3:
        particles.Triangle();
        break;
    }

    switch(integratorNumber) //Initialise Integrator
    {
        case 1:
            break;
        case 2:
            VerletIntegrator.Initiate(particles);
            break;
        case 3:
            break;
    }
}

void Pairwise::Update()
{
    switch(integratorNumber)
    {
    case 1: //Euler
        CalculateAccelerations(particles.X, particles.Y, particles.accelerationX, particles.accelerationY, particles.mass);
        EulerIntegrator.Update(particles, dt);
        particles.Draw();
        break;
    case 2: //Verlet
        VerletIntegrator.Update(particles, dt);
        particles.Draw();
        break;
    case 3: //RK4
        RK4Integrator.Update(particles, dt);
        particles.Draw();
        break;
    }
}


void Pairwise::CalculateAccelerations(Vec &positionX, Vec &positionY, Vec &accelerationX, Vec &accelerationY, Vec &mass)

{
    int numberOfParticles = positionX.size();
    accelerationX.setZero();
    accelerationY.setZero();
    
    #pragma omp parallel for
    for(int i = 0;  i < numberOfParticles; i++)
    {
        for(int j = i + 1; j < numberOfParticles; j++)
        {   
            double displacementX = positionX[j] - positionX[i];
            double displacementY = positionY[j] - positionY[i];

            std::array<double, 2> deltaAccelerations = CalculateAcceleration(displacementX, displacementY);

            double deltaAccelerationX = deltaAccelerations[0];
            double deltaAccelerationY = deltaAccelerations[1];

            accelerationX[i] += mass[j] * deltaAccelerationX;
            accelerationY[i] += mass[j] * deltaAccelerationY;

            accelerationX[j] -= mass[i] * deltaAccelerationX;
            accelerationY[j] -= mass[i] * deltaAccelerationY;
        }
    }
}

std::array<double, 2> Pairwise::CalculateAcceleration(double displacementX, double displacementY)

{
    double distanceSquared = displacementX*displacementX + displacementY*displacementY;
    double epsilon = 0.5;
    double denominator =  (distanceSquared + epsilon*epsilon);
    double factor = G / sqrt(denominator*denominator*denominator);
    double deltaAccelerationX = factor * displacementX;
    double deltaAccelerationY = factor * displacementY;

    return std::array<double, 2> {deltaAccelerationX, deltaAccelerationY};
}

