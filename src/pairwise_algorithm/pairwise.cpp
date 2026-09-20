#include "pairwise.hpp"
#include <random>
#include "integrator/Euler.hpp"
#include "integrator/Verlet.hpp"
#include "integrator/RK4.hpp"


PairwiseAlgorithm::PairwiseAlgorithm(int numberOfParticles, int integratorNum, int particleConfigNumber, double dt):
numberOfParticles(numberOfParticles),
integratorNum(integratorNum),
particleConfigNumber(particleConfigNumber),
particles(numberOfParticles),
RK4Integrator(numberOfParticles)
{
    initialiseParticles();
}

void PairwiseAlgorithm::initialiseParticles()
{
    //Initialising Configuration
    if (particleConfigNumber == 1) //Galaxy Configuration
    {
        particles.InitiataliseParticlesRadial();
    }
    else if (particleConfigNumber == 2) //Binary Galaxy
    {
        particles.BinaryGalaxy();
    }
    else if(particleConfigNumber == 3) //Triangle
    {
        particles.triangle();
    }

    //Initialising Integrator
    if (integratorNum == 1) // Euler
    {
        return;
    }
    else if(integratorNum == 2) // Verlet
    {
        VerletIntegrator.Initiate(particles);
    }
    else if(integratorNum == 3) //RK4
    {
        return;
    }


}

void PairwiseAlgorithm::Update(double dt)
{
    if(integratorNum == 1) //Euler
    {
        EulerIntegrator.Update(particles, dt);
        particles.Draw();
    }
    else if(integratorNum == 2) //Verlet
    {
        VerletIntegrator.Update(particles, dt);
        particles.Draw();
    }   
    else if(integratorNum == 3) //RK4
    {
        RK4Integrator.Update(particles, dt);
        particles.Draw();
    }
}


