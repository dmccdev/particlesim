#include "Integrator.hpp"


void Integrator::CalculateAccelerations(Vec &positionX, Vec &positionY, Vec &accelerationX, Vec &accelerationY, Vec &mass)

{
    int numberOfParticles = positionX.size();
    accelerationX.setZero();
    accelerationY.setZero();
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

std::array<double, 2> Integrator::CalculateAcceleration(double displacementX, double displacementY)

{
    double distanceSquared = displacementX*displacementX + displacementY*displacementY;
    double epsilon = 0.5;
    double denominator =  (distanceSquared + epsilon*epsilon);
    double factor = G / sqrt(denominator*denominator*denominator);
    double deltaAccelerationX = factor * displacementX;
    double deltaAccelerationY = factor * displacementY;

    return std::array<double, 2> {deltaAccelerationX, deltaAccelerationY};
}

