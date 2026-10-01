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
    int particlesCount;
    double G = 1.0;
    double pi = 3.14159265358979323846;
    ParticlesState(int particlesCount);
    ~ParticlesState();
    void PrintData();
    void BinaryStar();
    void SingleStar();
    void Galaxy();
    void Draw();
private:
    Color GetColorWhiteToRed(double speed, double maxSpeed);
    Texture2D particleTexture;
};