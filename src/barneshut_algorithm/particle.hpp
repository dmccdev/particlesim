#pragma once

class Particle
{
public:
    double X;
    double Y;

    double velocityX;
    double velocityY;

    double accelerationX;
    double accelerationY;

    double mass;
    Particle(double X, double Y, double velocityX, double velocityY, double mass);
};
