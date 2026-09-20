#include "Euler.hpp"


void Euler::Update(ParticlesState &particles, double dt)
{
    CalculateAccelerations(particles.X, particles.Y, particles.accelerationX, particles.accelerationY, particles.mass);

    particles.velocityX += particles.accelerationX * dt;
    particles.velocityY += particles.accelerationY * dt;

    particles.X += particles.velocityX * dt;
    particles.Y += particles.velocityY * dt;
}






