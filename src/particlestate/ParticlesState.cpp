#include "ParticlesState.hpp"
#include <random>
#include <raylib.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <rlgl.h>
#include <raymath.h>



ParticlesState::ParticlesState(int particlesCount) :
X(particlesCount), Y(particlesCount),
velocityX(particlesCount), velocityY(particlesCount),
accelerationX(particlesCount), accelerationY(particlesCount),
mass(particlesCount), speedSquared(particlesCount),
particlesCount(particlesCount)

{
    Image img = GenImageColor(16, 16, BLANK);
    ImageDrawCircle(&img, 8, 8, 8, WHITE);
    particleTexture = LoadTextureFromImage(img);
    UnloadImage(img);
}

ParticlesState::~ParticlesState()
{
    if (particleTexture.id != 0) {
        UnloadTexture(particleTexture);
    }
}

void ParticlesState::PrintData()// Debug

{
    std::cout << "Acceleration: " << '\n';  
    std::cout << accelerationX << '\n';  
    std::cout << accelerationY << '\n';  

    std::cout << "Velocity: " << '\n';  
    std::cout << velocityX << '\n';  
    std::cout << velocityY << '\n';  

    std::cout << "Position: " << '\n';  
    std::cout << X << '\n';  
    std::cout << Y << '\n';  

}

void ParticlesState::Galaxy()
{

    std::random_device rd;
    std::mt19937 generator64(rd());

    std::uniform_real_distribution<double> distributionRadius(150, 450);
    std::uniform_real_distribution<double> distributionTheta(0, 2 * pi);

    //Initialise Center Mass
    X[0] = 450.0;
    Y[0] = 450.0;
    mass[0] = 100000.0;
    velocityX[0] = 0.0;
    velocityY[0] = 0.0;

    for(int i = 1; i < particlesCount; i++)
    {
        //Position
        double radius = distributionRadius(generator64);
        double theta = distributionTheta(generator64);

        X[i] = X[0] + radius * std::cos(theta);
        Y[i] = Y[0] + radius * std::sin(theta);

        //Velocity
        double displacementX = X[i] - X[0];
        double displacementY = Y[i] - Y[0];

        double distance = sqrt(displacementX*displacementX + displacementY*displacementY);
        double speed = sqrt(G * mass[0] / distance);

        double tangentialVelocityX = -displacementY/distance * speed;
        double tangentialVelocityY = displacementX/distance * speed;

        velocityX[i] = tangentialVelocityX;
        velocityY[i] = tangentialVelocityY;
        mass[i] = 1.0;
    }
}

void ParticlesState::BinaryGalaxy()
{

    std::random_device rd;
    std::mt19937 generator64(rd());

    std::uniform_real_distribution<double> distributionRadius(150, 300);
    std::uniform_real_distribution<double> distributionTheta(0, 2 * pi);

    //Initialise Central Masses
    X[0] = 480.0;
    Y[0] = 450.0;
    mass[0] = 10000000.0;


    X[1] = 420.0;
    Y[1] = 450.0;
    mass[1] = 10000000.0;

    double centerMassX = (X[0] + X[1])/2;
    double centerMassY = (Y[0] + Y[1])/2;
    double centerMassMass = mass[0] + mass[1];

    //Calculating their velocities
    double distance = X[0] - X[1];
    double radius = distance/2;
    double speed = sqrt(G * mass[0] /( 4 * radius));

    velocityX[0] = 0.0;
    velocityX[1] = 0.0;

    velocityY[0] = speed; //Tangential velocity to center of mass in opposite directions
    velocityY[1] = -speed;

    for(int i = 2; i < particlesCount; i++)
    {
        //Position
        double radius = distributionRadius(generator64);
        double theta = distributionTheta(generator64);

        X[i] = centerMassX + radius * std::cos(theta);
        Y[i] = centerMassY + radius * std::sin(theta);

        //Velocity
        double displacementX = X[i] - centerMassX;
        double displacementY = Y[i] - centerMassY;

        double distance = sqrt(displacementX*displacementX + displacementY*displacementY);
        double speed = sqrt(G * centerMassMass / distance);

        double tangentialVelocityX = -displacementY/distance * speed;
        double tangentialVelocityY = displacementX/distance * speed;

        velocityX[i] = tangentialVelocityX;
        velocityY[i] = tangentialVelocityY;

        //Mass
        mass[i] = 1.0;
    }
}

void ParticlesState::Triangle()
{

    std::random_device rd;
    std::mt19937 generator64(rd());

    std::uniform_real_distribution<double> distributionRadius(100, 400);
    std::uniform_real_distribution<double> distributionTheta(0, 2*pi);
    std::uniform_real_distribution<double> distributionMass(0, 100);


    X[0] = 450.0;
    Y[0] = 450;
    mass[0] = 1000000.0;

    for(int i = 1; i < particlesCount; i++)
    {
        //Position
        double radius = distributionRadius(generator64);
        double theta = distributionTheta(generator64);

        X[i] = X[0] + radius * std::cos(theta);
        Y[i] = Y[0] + radius * std::sin(theta);

        //Velocity
        double displacementX = X[i] - X[0];
        double displacementY = Y[i] - Y[0];

        double distance = sqrt(displacementX*displacementX + displacementY*displacementY);
        double speed = sqrt(G * mass[0] / distance);

        double tangentialVelocityX = -displacementY/distance * speed;
        double tangentialVelocityY = displacementX/distance * speed;

        velocityX[i] = tangentialVelocityX;
        velocityY[i] = tangentialVelocityY;

        //Mass
        mass[i] = distributionMass(generator64);
    }
}

void ParticlesState::Draw()
{
    speedSquared = velocityX.array().square() + velocityY.array().square();
    double maxSpeedSquared = speedSquared.maxCoeff();
    if (particlesCount <= 0) return;

    double maxSpeed = speedSquared.maxCoeff();
    float pointSize = 2.0f; // Half-size = 1.0f

    BeginBlendMode(BLEND_ADDITIVE);
    rlBegin(RL_QUADS);
    for (int i = 0; i < particlesCount; i++)
    {
        if (i > 0 && (i % 2000 == 0)) 
        {
            rlEnd();
            rlBegin(RL_QUADS);
        }
        Color c = GetColorWhiteToRed(speedSquared[i], maxSpeedSquared);

        rlColor4ub(c.r, c.g, c.b, c.a);

        float px = static_cast<float>(X[i]);
        float py = static_cast<float>(Y[i]);

        // Draw a small 2x2 square quad for each particle
        rlVertex2f(px - 1.0f, py - 1.0f);
        rlVertex2f(px - 1.0f, py + 1.0f);
        rlVertex2f(px + 1.0f, py + 1.0f);
        rlVertex2f(px + 1.0f, py - 1.0f);
    }
    rlEnd();
    EndBlendMode();
}

Color ParticlesState::GetColorWhiteToRed(double speed, double maxSpeed) 
{
    float t = (float) std::clamp(speed, 0.0, maxSpeed) / maxSpeed;
    
    return ColorLerp(BLUE, RED, t);
}
