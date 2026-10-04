#pragma once

#include "particlestate/ParticlesState.hpp"



class Statistics
{
public:
int frame_rate;
double frameCalculationInterval;

double dt;
double G;
double epsilon;
double epsilonSquared;

double totalEnergy;
double energyError;

double linearMomentumX;
double linearMomentumY;
double linearMomentumErrorX;
double linearMomentumErrorY;


double angularMomentum;
double angularMomentumError;

double totalKineticEnergy;
double totalPotentialEnergy;

double initialTotalKineticEnergy;
double initialTotalPotentialEnergy;
double initialTotalEnergy;

double initialLinearMomentumX;
double initialLinearMomentumY;

double initialAngularMomentum;


Statistics(double dt, double G, double frameCalculationInterval, double epsilon);

void calculateStatistics(ParticlesState &particles);
void updateStatistics(ParticlesState &particles, int &frameCounter);
void captureInitialState(ParticlesState &particles);

private:
Vec initialPositionX;
Vec initialPositionY;
Vec initialVelocityX;
Vec initialVelocityY;
Vec initialMass;
bool initialStateCaptured = false;
bool initialStatisticsCalculated = false;

void calculateInitialStatistics();
};
