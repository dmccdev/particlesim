#pragma once

#include "particle.hpp"
#include "node.hpp"
#include <vector>
#include <raylib.h>
#include <cmath>

class QuadTree
{
public:
    double G = 0.1;
    double epsilon = 0.75;
    double theta = 0.5;
    double thetaSquared = theta*theta;
    double epsilonSquared = epsilon*epsilon;
    int numberOfParticles;
    std::vector<Node> nodes;
    std::vector<Particle> initialiseParticles(int numberOfParticles);

    void buildTree(std::vector<Particle> &particles, int centerScreenX, int centerScreenY);
    void Update(std::vector<Particle> &particles, double dt);
    void Draw(std::vector<Particle> &particle);
    void printData(std::vector<Particle> &particles);

private:

    int rootIndex = 0;

    void calculateNodeCentresOfMass(int nodeIndex, std::vector<Particle> &particles);
    void resetTree();
    bool empty(int nodeIndex); 
    bool external(int nodeIndex); 
    void calculateAcceleration(int nodeIndex, int particleIndex, std::vector<Particle> &particles);
    void insert(int nodeIndex, int particleIndex, std::vector<Particle> &particles);
    void subdivide(int nodeIndex);
    int assignQuadrant(int nodeIndex , Particle &particle); 
    bool checkInsideNodeQuadrant(int nodeIndex, Particle &particle);










};

