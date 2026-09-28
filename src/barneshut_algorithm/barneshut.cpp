#include "barneshut.hpp"
#include <omp.h>
#include <iostream>

BarnesHut::BarnesHut(int particleCount, int integratorNumber, int particleConfigNumber, double dt) :
particles(particleCount),
particleConfigNumber(particleConfigNumber),
integratorNumber(integratorNumber),
RK4Integrator(particleCount),
particleCount(particleCount),
dt(dt)

{
}

void BarnesHut::InitialiseParticles()
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

void BarnesHut::Update()
{

    particles.accelerationX.setZero();
    particles.accelerationY.setZero();

    quadtree.buildTree(particles, 450, 450);
    #pragma omp parallel for
    for(int particleIndex = 0; particleIndex < particleCount; particleIndex++)
    {
        calculateAcceleration(0, particleIndex);
    }

    switch(integratorNumber)
    {
        case 1:
            EulerIntegrator.Update(particles, dt);
            break;

        case 2:
            VerletIntegrator.Update(particles, dt);
            break;

        case 3:
            RK4Integrator.Update(particles, dt);
            break;
    }

    particles.Draw();
}



void BarnesHut::calculateAcceleration(int nodeIndex, int particleIndex)
{
    //Node External
    if(quadtree.external(nodeIndex))
    {
        //Node empty
        if(quadtree.empty(nodeIndex))
        {
            return;
        }
        //Node full
        else
        {
            int existingParticleIndex = quadtree.nodes[nodeIndex].particleIndex;
            if(existingParticleIndex == particleIndex)
            {
                return;
            }

            double displacementX = particles.X[existingParticleIndex] - particles.X[particleIndex]; 
            double displacementY = particles.Y[existingParticleIndex] - particles.Y[particleIndex];

            double distanceSquared = displacementX*displacementX + displacementY*displacementY;
            double denominator =  (distanceSquared + epsilonSquared);
            double factor = G / sqrt(denominator*denominator*denominator) * particles.mass[existingParticleIndex];

            double deltaAccelerationX = factor * displacementX;
            double deltaAccelerationY = factor * displacementY;

            particles.accelerationX[particleIndex] += deltaAccelerationX;
            particles.accelerationY[particleIndex] += deltaAccelerationY;

            return;
        }
    }
    //Node internal
    else
    {            
        double displacementX = quadtree.nodes[nodeIndex].centreMassX - particles.X[particleIndex]; 
        double displacementY = quadtree.nodes[nodeIndex].centreMassY - particles.Y[particleIndex];

        double distanceSquared = displacementX*displacementX + displacementY*displacementY; 
 
        if(distanceSquared == 0.0)
        {
            int childIndex = quadtree.nodes[nodeIndex].childFirstIndex;
            calculateAcceleration(childIndex, particleIndex);
            calculateAcceleration(childIndex+1, particleIndex);
            calculateAcceleration(childIndex+2, particleIndex);
            calculateAcceleration(childIndex+3, particleIndex);
            return;  
        }

        // double distance = sqrt(distanceSquared);
        double ratio = (4*quadtree.nodes[nodeIndex].halfWidth*quadtree.nodes[nodeIndex].halfWidth)/distanceSquared;


        if(ratio < theta)
        {
            double denominator =  (distanceSquared + epsilonSquared);
            double factor = G / sqrt(denominator*denominator*denominator) * quadtree.nodes[nodeIndex].mass;

            double deltaAccelerationX = factor * displacementX;
            double deltaAccelerationY = factor * displacementY;

            particles.accelerationX[particleIndex] += deltaAccelerationX;
            particles.accelerationY[particleIndex] += deltaAccelerationY;

        }
        else 
        {
            int childIndex = quadtree.nodes[nodeIndex].childFirstIndex;
            calculateAcceleration(childIndex, particleIndex);
            calculateAcceleration(childIndex+1, particleIndex);
            calculateAcceleration(childIndex+2, particleIndex);
            calculateAcceleration(childIndex+3, particleIndex);
            return;
        }
    }
}
