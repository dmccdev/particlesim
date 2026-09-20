#include "ParticlesState.hpp"
#include <random>
#include <raylib.h>
#include <iostream>
#include <algorithm>



ParticlesState::ParticlesState(int particlesCount) :
X(particlesCount), Y(particlesCount),
velocityX(particlesCount), velocityY(particlesCount),
accelerationX(particlesCount), accelerationY(particlesCount),
mass(particlesCount), speedSquared(particlesCount),
particlesCount(particlesCount)

{}

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

    std::uniform_real_distribution<double> distributionRadius(150, 300);
    std::uniform_real_distribution<double> distributionTheta(0, 2 * pi);

    //Initialise Center Mass
    X[particlesCount-1] = 450.0;
    Y[particlesCount-1] = 450.0;
    mass[particlesCount-1] = 100000.0;
    velocityX[particlesCount-1] = 0.0;
    velocityY[particlesCount-1] = 0.0;

    for(int i = 0; i < particlesCount-1; i++)
    {
        //Position
        double radius = distributionRadius(generator64);
        double theta = distributionTheta(generator64);

        X[i] = 450 + radius * std::cos(theta);
        Y[i] = 450 + radius * std::sin(theta);

        //Velocity
        double displacementX = X[i] - X[particlesCount-1];
        double displacementY = Y[i] - Y[particlesCount-1];

        double distance = sqrt(displacementX*displacementX + displacementY*displacementY);
        double speed = sqrt(G * mass[particlesCount-1] / distance);

        double tangentialVelocityX = -displacementY/distance * speed;
        double tangentialVelocityY = displacementX/distance * speed;

        velocityX[i] = tangentialVelocityX;
        velocityY[i] = tangentialVelocityY;

        //Mass
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
    X[0] = 450.0;
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

        X[i] = 450 + radius * std::cos(theta);
        Y[i] = 450 + radius * std::sin(theta);

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

    int radius = 3;
    int size = radius * 2;


    RenderTexture2D circleTex = LoadRenderTexture(32, 32);
    BeginTextureMode(circleTex);
        ClearBackground(BLANK);
        DrawCircle(16, 16, 16, WHITE); 
    EndTextureMode();


    Rectangle source = {0, 0, (float)size, (float)-size}; 
    Vector2 origin = {0, 0};


    speedSquared.setZero();
    speedSquared = velocityX.array().square() + velocityY.array().square();

    double maxSpeed = speedSquared.maxCoeff(); 
    for(int i = 0; i < particlesCount; i++)
    { 
        Rectangle dest = {(float) X[i] - radius, (float)Y[i] - radius, (float)size, (float)size};
        Color speedColour = GetColorWhiteToRed(speedSquared[i], maxSpeed);
        DrawTexturePro(circleTex.texture, source, dest, origin, 0.0f, speedColour);
    }
}


Color ParticlesState::GetColorWhiteToRed(double speed, double maxSpeed) 
{
    float t = (float) std::clamp(speed, 0.0, maxSpeed) / maxSpeed;
    
    return ColorLerp(BLUE, RED, t);
}
