#include <iostream>
#include <Eigen/Dense>
#include <array>


struct Particle
{
    double X;
    double Y;

    double velocityX;
    double velocityY;

    double accelerationX;
    double accelerationY;

    double mass;
};

struct Node
{
    int particleIndex = -1;
    int childFirstIndex = -1;
    double centreMassX = 0;
    double centreMassY = 0;

    double mass = 0; 

    double halfWidth;

    double squareCenterX;
    double squareCenterY;
};

struct QuadTree
{
    int rootIndex = 0;
    double G = 1.0;
    double epsilon = 0.5;
    double theta = 0.5;
    std::vector<Node> nodes;

    int assignQuadrant(int nodeIndex , Particle &particle) //Returns 0,1,2,3 depending on quadrant
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
    bool empty(int nodeIndex) //Returns true if contains no particle
    {
        return (nodes[nodeIndex].particleIndex == -1);
    }

    bool external(int nodeIndex) //Returns true if external (no children nodes)
    {
        return (nodes[nodeIndex].childFirstIndex == -1);
    }

    void buildTree(std::vector<Particle> &particles, int centerScreenX, int centerScreenY)
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
    }

    void subdivide(int nodeIndex)
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

    void resetTree()
    {
        nodes.clear();
    }

    void insert(int nodeIndex, int particleIndex, std::vector<Particle> &particles)
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

    void calculateAcceleration(int nodeIndex, int particleIndex, std::vector<Particle> &particles)
    {
        // //Set Acceleration to zero
        // particles[particleIndex].accelerationX = 0;
        // particles[particleIndex].accelerationY = 0;

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

            double distance = sqrt(distanceSquared);
            double ratio = (2*nodes[nodeIndex].halfWidth)/distance;


            if(ratio < theta)
            {
                double denominator =  (distanceSquared + epsilon*epsilon);
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

    bool checkInsideNodeQuadrant(int nodeIndex, Particle &particle)
    {
        bool left = nodes[nodeIndex].squareCenterX - nodes[nodeIndex].halfWidth >= particle.X;
        bool right = nodes[nodeIndex].squareCenterX + nodes[nodeIndex].halfWidth < particle.X;
        bool above = nodes[nodeIndex].squareCenterY - nodes[nodeIndex].halfWidth >= particle.Y;
        bool below = nodes[nodeIndex].squareCenterY + nodes[nodeIndex].halfWidth < particle.Y;
        bool insideX =  !left && !right && !above && !below;

        return insideX;
    }

    void Update(std::vector<Particle> &particles, double dt)
    {
        for(int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
        {
            particles[particleIndex].accelerationX = 0.0;
            particles[particleIndex].accelerationY = 0.0;

            calculateAcceleration(rootIndex, particleIndex, particles);
        }
        for(Particle particle: particles)
        {
            particle.velocityX += dt * particle.accelerationX;
            particle.velocityY += dt * particle.accelerationY;

            particle.X += dt * particle.velocityX;
            particle.Y += dt * particle.velocityY;
        }
    }

    void calculateNodeCentresOfMass(int nodeIndex, std::vector<Particle> &particles)
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
};

