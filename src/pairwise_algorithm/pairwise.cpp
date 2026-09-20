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
RK4Integrator(particlesCount)
{
    initialiseParticles();
}

void Pairwise::initialiseParticles()
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

void Pairwise::Update(double dt)
{
    switch(integratorNumber)
    {
    case 1: //Euler
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


