#include "Euler.hpp"


void Euler::Update(ParticlesState &particles, double dt)
{
    particles.velocityX += particles.accelerationX * dt;
    particles.velocityY += particles.accelerationY * dt;

    particles.X += particles.velocityX * dt;
    particles.Y += particles.velocityY * dt;
}






