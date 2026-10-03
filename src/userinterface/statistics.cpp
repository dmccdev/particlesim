#include "statistics.hpp"

Statistics::Statistics(double dt, double G, double frameCalculationInterval)
{
    this->dt = dt;
    this->G = G;
    this->frameCalculationInterval = frameCalculationInterval;

    initialAngularMomentum = 0.0;
    initialLinearMomentumX = 0.0;
    initialLinearMomentumY = 0.0;
    initialTotalEnergy = 0.0;
    initialTotalKineticEnergy = 0.0;
    initialTotalPotentialEnergy = 0.0;
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

    //Calculate Linear Momentum, Angular Momentum & Total Kinetic Energy
    #ifdef USE_OPENMP
    #pragma omp parallel for
    #endif

    for(int i = 0; i < numberOfParticles; i++)
    {
        linearMomentumX += particles.mass[i] * particles.velocityX[i];
        linearMomentumY += particles.mass[i] * particles.velocityY[i];

        angularMomentum += particles.mass[i] * (particles.X[i] * particles.velocityY[i] - particles.Y[i] * particles.velocityX[i]);

        totalKineticEnergy += 0.5 * particles.mass[i] * (particles.velocityX[i] * particles.velocityX[i] + particles.velocityY[i] * particles.velocityY[i]);
    }


    //Calculating Potential Energy
    #ifdef USE_OPENMP
    #pragma omp parallel for
    #endif

    for(int i = 0;  i < numberOfParticles; i++)
    {
        for(int j = i + 1; j < numberOfParticles; j++)
        {   
            double displacementX = particles.X[j] - particles.X[i];
            double displacementY = particles.Y[j] - particles.Y[i];

            double distance = sqrtf(displacementX*displacementX + displacementY*displacementY);
            if(distance > 0)
            {
                totalPotentialEnergy += -1 * G * particles.mass[i] * particles.mass[j] / distance;
            }
        }
    }

    //Calculating Total Energy
    totalEnergy = totalKineticEnergy + totalPotentialEnergy;
    if(initialTotalEnergy > 0)
    {
        energyError = abs(totalEnergy - initialTotalEnergy) / abs(initialTotalEnergy) * 100;
    }

    linearMomentumErrorX = abs(linearMomentumX - initialLinearMomentumX);
    linearMomentumErrorY = abs(linearMomentumY - initialLinearMomentumY);

    angularMomentumError = abs(angularMomentum - initialAngularMomentum);
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

    //Calculating Inital Linear Momentum, Initial Angular Energy & Initial Total Kinetic Energy
    #ifdef USE_OPENMP
    #pragma omp parallel for
    #endif
    
    for(int i = 0; i < numberOfParticles; i++)
    {
        initialLinearMomentumX += particles.mass[i] * particles.velocityX[i];
        initialLinearMomentumY += particles.mass[i] * particles.velocityY[i];

        initialAngularMomentum += particles.mass[i] * (particles.X[i] * particles.velocityY[i] - particles.Y[i] * particles.velocityX[i]);

        initialTotalKineticEnergy += 0.5 * particles.mass[i] * (particles.velocityX[i] * particles.velocityX[i] + particles.velocityY[i] * particles.velocityY[i]);
    }


    //Calculating Initial Potential Energy
    #ifdef USE_OPENMP
    #pragma omp parallel for
    #endif

    for(int i = 0;  i < numberOfParticles; i++)
    {
        for(int j = i + 1; j < numberOfParticles; j++)
        {   
            double displacementX = particles.X[j] - particles.X[i];
            double displacementY = particles.Y[j] - particles.Y[i];

            double distance = sqrtf(displacementX*displacementX + displacementY*displacementY);
            if(distance > 0)
            {
                initialTotalPotentialEnergy += -1 * G * particles.mass[i] * particles.mass[j] / distance; 
            }
        }
    }

    //Calculating Initial Total Energy
    initialTotalEnergy = initialTotalPotentialEnergy + initialTotalKineticEnergy;
}
