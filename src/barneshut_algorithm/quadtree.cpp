#include "quadtree.hpp"
#include <iostream>
#include <random>
#include <omp.h>


QuadTree::QuadTree()
{
    nodes.reserve(20000);
}


int QuadTree::assignQuadrant(int nodeIndex, double particleX, double particleY) //Needs fixing
{
    bool above = particleY < nodes[nodeIndex].squareCenterY;
    bool right = particleX >= nodes[nodeIndex].squareCenterX;

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


void QuadTree::buildTree(ParticlesState &particles, int centerScreenX, int centerScreenY)
{
    resetTree();
        
    //Make Root Node
    nodes.resize(1);
    nodes[rootIndex].squareCenterX = centerScreenX;
    nodes[rootIndex].squareCenterY = centerScreenY;
    nodes[rootIndex].halfWidth = centerScreenX;

    for(int particleIndex = 0; particleIndex < particles.particlesCount; particleIndex++)
    {
        insert(rootIndex, particleIndex, particles);
    }

    //Calculate center of mass
    calculateNodeCentresOfMass(rootIndex, particles);
}

void QuadTree::subdivide(int nodeIndex)
{
    int nextAvailableIndex = nodes.size(); //The final index

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

void QuadTree::insert(int nodeIndex, int particleIndex, ParticlesState &particles)
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
            int existingQuadrantNumber = assignQuadrant(nodeIndex, particles.X[existingParticleIndex], particles.Y[existingParticleIndex]);
            int QuadrantNumber = assignQuadrant(nodeIndex, particles.X[particleIndex], particles.Y[particleIndex]);


            insert(nodes[nodeIndex].childFirstIndex + existingQuadrantNumber, existingParticleIndex, particles);
            insert(nodes[nodeIndex].childFirstIndex + QuadrantNumber, particleIndex, particles);
        }    
    }
    //Node internal
    else
    {
        int quadrantNumber = assignQuadrant(nodeIndex, particles.X[particleIndex], particles.Y[particleIndex]);
        insert(nodes[nodeIndex].childFirstIndex + quadrantNumber, particleIndex, particles);
    }
}

bool QuadTree::checkInsideNodeQuadrant(int nodeIndex, double particleX, double particleY)
{
    bool left = nodes[nodeIndex].squareCenterX - nodes[nodeIndex].halfWidth >= particleX;
    bool right = nodes[nodeIndex].squareCenterX + nodes[nodeIndex].halfWidth < particleX;
    bool above = nodes[nodeIndex].squareCenterY - nodes[nodeIndex].halfWidth >= particleY;
    bool below = nodes[nodeIndex].squareCenterY + nodes[nodeIndex].halfWidth < particleY;
    bool insideX =  !left && !right && !above && !below;

    return insideX;
}


void QuadTree::calculateNodeCentresOfMass(int nodeIndex, ParticlesState &particles)
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
            nodes[nodeIndex].centreMassX = particles.X[nodes[nodeIndex].particleIndex];
            nodes[nodeIndex].centreMassY = particles.Y[nodes[nodeIndex].particleIndex];

            nodes[nodeIndex].mass = particles.mass[nodes[nodeIndex].particleIndex];
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

        //Divide by total mass for final center of mass bit

        nodes[nodeIndex].centreMassX /= nodes[nodeIndex].mass;
        nodes[nodeIndex].centreMassY /= nodes[nodeIndex].mass;

        return;
    }
}



void QuadTree::printData(ParticlesState &particles) //Debug func
{
    std::cout << "Particles acceleration: " << '\n';
    for(int i = 0; i < numberOfParticles; i++)
    {
        std::cout << particles.accelerationX[i] << ", " << particles.accelerationY[i] << '\n';
    }

    std::cout << "Particles velocity: " << '\n';
    for(int i = 0; i < numberOfParticles; i++)
    {
        std::cout << particles.velocityX[i] << ", " << particles.velocityY[i] << '\n';
    }

    std::cout << "Particles position: " << '\n';
    for(int i = 0; i < numberOfParticles; i++)
    {
        std::cout << particles.X[i] << ", " << particles.Y[i] << '\n';
    }
}

