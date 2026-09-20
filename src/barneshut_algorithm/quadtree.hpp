#pragma once

#include "particle.hpp"
#include "node.hpp"
#include <vector>
#include <raylib.h>
#include <cmath>

class QuadTree
{
public:
    int rootIndex = 0;
    double G = 0.1;
    double epsilon = 0.75;
    double theta = 0.5;
    double thetaSquared = theta*theta;
    double epsilonSquared = epsilon*epsilon;
    int numberOfParticles;
    std::vector<Node> nodes;
    int assignQuadrant(int nodeIndex , Particle &particle); //Returns 0,1,2,3 depending on quadrant
    bool empty(int nodeIndex); //Returns true if contains no particle
    bool external(int nodeIndex); //Returns true if external (no children nodes)
    void buildTree(std::vector<Particle> &particles, int centerScreenX, int centerScreenY);
    void subdivide(int nodeIndex);
    void resetTree();
    void insert(int nodeIndex, int particleIndex, std::vector<Particle> &particles);
    void calculateAcceleration(int nodeIndex, int particleIndex, std::vector<Particle> &particles);
    bool checkInsideNodeQuadrant(int nodeIndex, Particle &particle);
    void Update(std::vector<Particle> &particles, double dt);
    void calculateNodeCentresOfMass(int nodeIndex, std::vector<Particle> &particles);
    void Draw(std::vector<Particle> &particle);
    void printData(std::vector<Particle> &particles);
    std::vector<Particle> initialiseParticles(int numberOfParticles);
};
