#pragma once

#include "particlestate/ParticlesState.hpp"
#include "node.hpp"
#include <vector>
#include <raylib.h>
#include <cmath>

class QuadTree
{
public:

    QuadTree();

    int numberOfParticles;
    std::vector<Node> nodes;
    
    void buildTree(ParticlesState &particles, int centerScreenX, int centerScreenY);
    void printData(ParticlesState &particles);

    void calculateNodeCentresOfMass(int nodeIndex, ParticlesState &particles);
    void resetTree();
    bool empty(int nodeIndex); 
    bool external(int nodeIndex); 
    void calculateAcceleration(int nodeIndex, int particleIndex, ParticlesState &particles);
    void insert(int nodeIndex, int particleIndex, ParticlesState &particles);
    void subdivide(int nodeIndex);
    int assignQuadrant(int nodeIndex , double particleX, double particleY); 
    bool checkInsideNodeQuadrant(int nodeIndex, double particleX, double particleY);

private:

    int rootIndex = 0;











};

