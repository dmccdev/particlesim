#pragma once
#include <Eigen/Dense>
#include <raylib.h>


using Vec = Eigen::VectorXd;

class ParticlesState
{
public:
    Vec X;
    Vec Y;
    Vec velocityX;
    Vec velocityY;
    Vec accelerationX;
    Vec accelerationY;
    Vec mass;
    Vec speedSquared;
    int numberOfParticles;

    ParticlesState(int numberOfParticles);
    
    void PrintData();
    void InitiataliseParticlesRadial();
    void BinaryGalaxy();
    void triangle();
    Color GetColorWhiteToRed(double speed, double maxSpeed);
    void Draw();
};