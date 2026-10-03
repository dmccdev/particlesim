#include "statistics.hpp"
#include <iostream>
#include <cmath>

Statistics::Statistics(double dt, double G, double frameCalculationInterval, double epsilon)
{
    this->dt = dt;
    this->G = G;
    this->frameCalculationInterval = frameCalculationInterval;
    this->epsilon = epsilon;
    this->epsilonSquared = epsilon*epsilon;
}

void Statistics::calculateStatistics(ParticlesState &particles)
{
    //Reset Statistics
    linearMomentumX = 0.0;
    linearMomentumY = 0.0;
    angularMomentum = 0.0;
    totalEnergy = 0.0;
    totalKineticEnergy = 0.0;
    totalPotentialEnergy = 0.0;
    energyError = 0;

    //Calculate Linear Momentum, Angular Momentum & Total Kinetic Energy
    #ifdef USE_OPENMP
    #pragma omp parallel for reduction(+:linearMomentumX,linearMomentumY,angularMomentum,totalKineticEnergy)
    #endif

    for(int i = 0; i < particles.particlesCount; i++)
    {
        linearMomentumX += particles.mass[i] * particles.velocityX[i];
        linearMomentumY += particles.mass[i] * particles.velocityY[i];

        angularMomentum += particles.mass[i] * (particles.X[i] * particles.velocityY[i] - particles.Y[i] * particles.velocityX[i]);

        totalKineticEnergy += 0.5 * particles.mass[i] * (particles.velocityX[i] * particles.velocityX[i] + particles.velocityY[i] * particles.velocityY[i]);
    }


    //Calculating Potential Energy
    for(int i = 0;  i < particles.particlesCount; i++)
    {
        for(int j = i + 1; j < particles.particlesCount; j++)
        {   
            double displacementX = particles.X[j] - particles.X[i];
            double displacementY = particles.Y[j] - particles.Y[i];

            double distanceSquared = displacementX*displacementX + displacementY*displacementY;
            if(distanceSquared > 0)
            {
                totalPotentialEnergy -= G * particles.mass[i] * particles.mass[j] / std::sqrt(distanceSquared + epsilonSquared);
            }
        }
    }   

    //Calculating Total Energy
    totalEnergy = totalKineticEnergy + totalPotentialEnergy;
    //Calculating Energy Error
    energyError = std::abs((totalEnergy - initialTotalEnergy) / initialTotalEnergy) * 100;
    

    linearMomentumErrorX = std::abs(linearMomentumX - initialLinearMomentumX);
    linearMomentumErrorY = std::abs(linearMomentumY - initialLinearMomentumY);

    angularMomentumError = std::abs(angularMomentum - initialAngularMomentum);
}

void Statistics::updateStatistics(ParticlesState &particles, int &frameCounter)
{
    if(frameCounter >= frameCalculationInterval)
    {
        calculateStatistics(particles);
        frameCounter = 0;
    }
}

void Statistics::calculateInitialStatistics(ParticlesState &particles)
{
    initialLinearMomentumX = 0.0;
    initialLinearMomentumY = 0.0;
    initialAngularMomentum = 0.0;
    initialTotalKineticEnergy = 0.0;
    initialTotalPotentialEnergy = 0.0;
    initialTotalEnergy = 0.0;

    //Calculating Inital Linear Momentum, Initial Angular Energy & Initial Total Kinetic Energy
    #ifdef USE_OPENMP
    #pragma omp parallel for reduction(+:initialLinearMomentumX,initialLinearMomentumY,initialAngularMomentum,initialTotalKineticEnergy)
    #endif

    for(int i = 0; i < particles.particlesCount; i++)
    {
        initialLinearMomentumX += particles.mass[i] * particles.velocityX[i];
        initialLinearMomentumY += particles.mass[i] * particles.velocityY[i];

        initialAngularMomentum += particles.mass[i] * (particles.X[i] * particles.velocityY[i] - particles.Y[i] * particles.velocityX[i]);

        initialTotalKineticEnergy += 0.5 * particles.mass[i] * (particles.velocityX[i] * particles.velocityX[i] + particles.velocityY[i] * particles.velocityY[i]);
    }


    //Calculating Initial Potential Energy
    for(int i = 0;  i < particles.particlesCount; i++)
    {
        for(int j = i + 1; j < particles.particlesCount; j++)
        {   
            double displacementX = particles.X[j] - particles.X[i];
            double displacementY = particles.Y[j] - particles.Y[i];

            double distanceSquared = displacementX*displacementX + displacementY*displacementY;
            if(distanceSquared > 0)
            {
                initialTotalPotentialEnergy -= G * particles.mass[i] * particles.mass[j] / std::sqrt(distanceSquared + epsilonSquared); 
            }
        }
    }
    //Calculating Initial Total Energy
    initialTotalEnergy = initialTotalPotentialEnergy + initialTotalKineticEnergy;
}
