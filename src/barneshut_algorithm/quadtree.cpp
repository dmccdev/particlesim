#include "quadtree.hpp"
#include <iostream>
#include <random>
#include <omp.h>

int QuadTree::assignQuadrant(int nodeIndex, Particle &particle)
{
    bool above = particle.Y < nodes[nodeIndex].squareCenterY;
    bool right = particle.X >= nodes[nodeIndex].squareCenterX;

    if(above && !right)
        {return 0;}
    else if(above && right)
        {return 1;}
    else if(!above && !right)
        {return 2;}
    else
        {return 3;}
}

bool QuadTree::empty(int nodeIndex)
{
    return (nodes[nodeIndex].particleIndex == -1);
}

bool QuadTree::external(int nodeIndex)
{
    return (nodes[nodeIndex].childFirstIndex == -1);
}

void QuadTree::buildTree(std::vector<Particle> &particles, int centerScreenX, int centerScreenY)
{
    resetTree();
        
    //Make Root Node
    nodes.resize(1);
    nodes[rootIndex].squareCenterX = centerScreenX;
    nodes[rootIndex].squareCenterY = centerScreenY;
    nodes[rootIndex].halfWidth = centerScreenX;

    for(int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
    {
        insert(rootIndex, particleIndex, particles);
    }

    //Calculate center of mass
    calculateNodeCentresOfMass(rootIndex, particles);
}

void QuadTree::subdivide(int nodeIndex)
{
    int nextAvailableIndex = nodes.size();

    nodes.resize(nodes.size() + 4);
    nodes[nodeIndex].childFirstIndex = nextAvailableIndex;

    double parentSquareCenterX = nodes[nodeIndex].squareCenterX;
    double parentSquareCenterY = nodes[nodeIndex].squareCenterY;
    double parentHalfWidth = nodes[nodeIndex].halfWidth;
    double childHalfWidth = parentHalfWidth * 0.5;

    //Initialise Children Nodes
    nodes[nextAvailableIndex].squareCenterX = parentSquareCenterX - childHalfWidth;
    nodes[nextAvailableIndex].squareCenterY = parentSquareCenterY - childHalfWidth;
    nodes[nextAvailableIndex].halfWidth = childHalfWidth;

    nodes[nextAvailableIndex+1].squareCenterX = parentSquareCenterX + childHalfWidth;
    nodes[nextAvailableIndex+1].squareCenterY = parentSquareCenterY - childHalfWidth;
    nodes[nextAvailableIndex+1].halfWidth = childHalfWidth;

    nodes[nextAvailableIndex+2].squareCenterX = parentSquareCenterX - childHalfWidth;
    nodes[nextAvailableIndex+2].squareCenterY = parentSquareCenterY + childHalfWidth;
    nodes[nextAvailableIndex+2].halfWidth = childHalfWidth;

    nodes[nextAvailableIndex+3].squareCenterX = parentSquareCenterX + childHalfWidth;
    nodes[nextAvailableIndex+3].squareCenterY = parentSquareCenterY + childHalfWidth;
    nodes[nextAvailableIndex+3].halfWidth = childHalfWidth;
}

void QuadTree::resetTree()
{
    nodes.clear();
}

void QuadTree::insert(int nodeIndex, int particleIndex, std::vector<Particle> &particles)
{

    if(external(nodeIndex))
    {
        //Node external and empty
        if(empty(nodeIndex))
        {
            nodes[nodeIndex].particleIndex = particleIndex;
            return;
        }
        //Node external and full
        else
        {
            if(nodes[nodeIndex].halfWidth < 0.001)
            {
                return;
            }
            subdivide(nodeIndex);
            int existingParticleIndex = nodes[nodeIndex].particleIndex;
            nodes[nodeIndex].particleIndex = -1; //Make parent node empty again
            int existingQuadrantNumber = assignQuadrant(nodeIndex, particles[existingParticleIndex]);
            int QuadrantNumber = assignQuadrant(nodeIndex, particles[particleIndex]);


            insert(nodes[nodeIndex].childFirstIndex + existingQuadrantNumber, existingParticleIndex, particles);
            insert(nodes[nodeIndex].childFirstIndex + QuadrantNumber, particleIndex, particles);
        }    
    }
    //Node internal
    else
    {
        int quadrantNumber = assignQuadrant(nodeIndex, particles[particleIndex]);
        insert(nodes[nodeIndex].childFirstIndex + quadrantNumber, particleIndex, particles);
    }
}

void QuadTree::calculateAcceleration(int nodeIndex, int particleIndex, std::vector<Particle> &particles)
{
    //Node External
    if(external(nodeIndex))
    {
        //Node empty
        if(empty(nodeIndex))
        {
            return;
        }
        //Node full
        else
        {
            int existingParticleIndex = nodes[nodeIndex].particleIndex;
            if(existingParticleIndex == particleIndex)
            {
                return;
            }

            double displacementX = particles[existingParticleIndex].X - particles[particleIndex].X; 
            double displacementY = particles[existingParticleIndex].Y - particles[particleIndex].Y;

            double distanceSquared = displacementX*displacementX + displacementY*displacementY;
            double denominator =  (distanceSquared + epsilon*epsilon);
            double factor = G / sqrt(denominator*denominator*denominator) * particles[existingParticleIndex].mass;

            double deltaAccelerationX = factor * displacementX;
            double deltaAccelerationY = factor * displacementY;

            particles[particleIndex].accelerationX += deltaAccelerationX;
            particles[particleIndex].accelerationY += deltaAccelerationY;

            return;
        }
    }
    //Node internal
    else
    {            
        double displacementX = nodes[nodeIndex].centreMassX - particles[particleIndex].X; 
        double displacementY = nodes[nodeIndex].centreMassY - particles[particleIndex].Y;

        double distanceSquared = displacementX*displacementX + displacementY*displacementY; 
 
        if(distanceSquared == 0.0)
        {
            int childIndex = nodes[nodeIndex].childFirstIndex;
            calculateAcceleration(childIndex, particleIndex, particles);
            calculateAcceleration(childIndex+1, particleIndex, particles);
            calculateAcceleration(childIndex+2, particleIndex, particles);
            calculateAcceleration(childIndex+3, particleIndex, particles);
            return;  
        }

        // double distance = sqrt(distanceSquared);
        double ratio = (4*nodes[nodeIndex].halfWidth*nodes[nodeIndex].halfWidth)/distanceSquared;


        if(ratio < theta)
        {
            double denominator =  (distanceSquared + epsilonSquared);
            double factor = G / sqrt(denominator*denominator*denominator) * nodes[nodeIndex].mass;

            double deltaAccelerationX = factor * displacementX;
            double deltaAccelerationY = factor * displacementY;

            particles[particleIndex].accelerationX += deltaAccelerationX;
            particles[particleIndex].accelerationY += deltaAccelerationY;

        }
        else 
        {
            int childIndex = nodes[nodeIndex].childFirstIndex;
            calculateAcceleration(childIndex, particleIndex, particles);
            calculateAcceleration(childIndex+1, particleIndex, particles);
            calculateAcceleration(childIndex+2, particleIndex, particles);
            calculateAcceleration(childIndex+3, particleIndex, particles);
            return;
        }
    }
}

bool QuadTree::checkInsideNodeQuadrant(int nodeIndex, Particle &particle)
{
    bool left = nodes[nodeIndex].squareCenterX - nodes[nodeIndex].halfWidth >= particle.X;
    bool right = nodes[nodeIndex].squareCenterX + nodes[nodeIndex].halfWidth < particle.X;
    bool above = nodes[nodeIndex].squareCenterY - nodes[nodeIndex].halfWidth >= particle.Y;
    bool below = nodes[nodeIndex].squareCenterY + nodes[nodeIndex].halfWidth < particle.Y;
    bool insideX =  !left && !right && !above && !below;

    return insideX;
}

void QuadTree::Update(std::vector<Particle> &particles, double dt)
{
    #pragma omp parallel for
    for(int particleIndex = 0; particleIndex < numberOfParticles; particleIndex++)
    {
        particles[particleIndex].accelerationX = 0.0;
        particles[particleIndex].accelerationY = 0.0;

        calculateAcceleration(rootIndex, particleIndex, particles);
    
        particles[particleIndex].velocityX += dt * particles[particleIndex].accelerationX;
        particles[particleIndex].velocityY += dt * particles[particleIndex].accelerationY;

        particles[particleIndex].X += dt * particles[particleIndex].velocityX;
        particles[particleIndex].Y += dt * particles[particleIndex].velocityY;
    }
}

void QuadTree::calculateNodeCentresOfMass(int nodeIndex, std::vector<Particle> &particles)
{
    if(external(nodeIndex)) //Node External
    {
        if(empty(nodeIndex)) //Node Empty
        {
            return;
        }
        else //Node Full
        {
            //Assign Node with particle position and mass
            nodes[nodeIndex].centreMassX = particles[nodes[nodeIndex].particleIndex].X;
            nodes[nodeIndex].centreMassY = particles[nodes[nodeIndex].particleIndex].Y;

            nodes[nodeIndex].mass = particles[nodes[nodeIndex].particleIndex].mass;
            return;
        }
    }
    else
    {
        //Recursive - call function for child nodes

        int childIndex = nodes[nodeIndex].childFirstIndex;
        calculateNodeCentresOfMass(childIndex, particles);
        calculateNodeCentresOfMass(childIndex+1, particles);
        calculateNodeCentresOfMass(childIndex+2, particles);
        calculateNodeCentresOfMass(childIndex+3, particles);

        //Compute center of mass numerator

        nodes[nodeIndex].centreMassX += nodes[childIndex].mass * nodes[childIndex].centreMassX; 
        nodes[nodeIndex].centreMassX += nodes[childIndex+1].mass * nodes[childIndex+1].centreMassX; 
        nodes[nodeIndex].centreMassX += nodes[childIndex+2].mass * nodes[childIndex+2].centreMassX; 
        nodes[nodeIndex].centreMassX += nodes[childIndex+3].mass * nodes[childIndex+3].centreMassX;

        nodes[nodeIndex].centreMassY += nodes[childIndex].mass * nodes[childIndex].centreMassY; 
        nodes[nodeIndex].centreMassY += nodes[childIndex+1].mass * nodes[childIndex+1].centreMassY; 
        nodes[nodeIndex].centreMassY += nodes[childIndex+2].mass * nodes[childIndex+2].centreMassY; 
        nodes[nodeIndex].centreMassY += nodes[childIndex+3].mass * nodes[childIndex+3].centreMassY; 

        //Compute total mass from children nodes

        nodes[nodeIndex].mass += nodes[childIndex].mass;
        nodes[nodeIndex].mass += nodes[childIndex+1].mass;
        nodes[nodeIndex].mass += nodes[childIndex+2].mass;
        nodes[nodeIndex].mass += nodes[childIndex+3].mass;

        //Divide by total mass for true center of mass

        nodes[nodeIndex].centreMassX /= nodes[nodeIndex].mass;
        nodes[nodeIndex].centreMassY /= nodes[nodeIndex].mass;

        return;
    }
}

void QuadTree::Draw(std::vector<Particle> &particles)
{
    for(int i = 0; i < numberOfParticles; i++)
    {
        DrawPixel(particles[i].X, particles[i].Y, WHITE);
    }
}

void QuadTree::printData(std::vector<Particle> &particles) //Debug func
{
    std::cout << "Particles acceleration: " << '\n';
    for(int i = 0; i < numberOfParticles; i++)
    {
        std::cout << particles[i].accelerationX << ", " << particles[i].accelerationY << '\n';
    }

    std::cout << "Particles velocity: " << '\n';
    for(int i = 0; i < numberOfParticles; i++)
    {
        std::cout << particles[i].velocityX << ", " << particles[i].velocityY << '\n';
    }

    std::cout << "Particles position: " << '\n';
    for(int i = 0; i < numberOfParticles; i++)
    {
        std::cout << particles[i].X << ", " << particles[i].Y << '\n';
    }
}

std::vector<Particle> QuadTree::initialiseParticles(int numberOfParticles)
{
    this->numberOfParticles = numberOfParticles; 
    //Initialise a N length vector
    std::vector<Particle> particles;
    particles.reserve(numberOfParticles);
    
    std::random_device rd;
    std::mt19937 generator64(rd());

    std::uniform_real_distribution<double> distributionRadius(100, 400);
    std::uniform_real_distribution<double> distributionTheta(0, 2 * PI);

    //Initialise Center Mass
    particles.push_back(Particle(450.0, 450.0, 0.0, 0.0, 10000.0));

    for(int i = 1; i < numberOfParticles; i++)
    {
        //Calculating random Position
        double radius = distributionRadius(generator64);
        double theta = distributionTheta(generator64);

        double X = 450 + radius * std::cos(theta);
        double Y = 450 + radius * std::sin(theta);

        //Calculating random Velocity
        double displacementX = X - 450.0;
        double displacementY = Y - 450.0;

        double distance = sqrt(displacementX*displacementX + displacementY*displacementY);
        double speed = sqrt(G * particles[0].mass / distance);

        double tangentialVelocityX = -displacementY/distance * speed;
        double tangentialVelocityY = displacementX/distance * speed;

        double velocityX = tangentialVelocityX;
        double velocityY = tangentialVelocityY;

        particles.push_back(Particle(X, Y, velocityX, velocityY, 1.0));
    }
    return particles;
}
