#pragma once

#include "particlestate/ParticlesState.hpp"



class Statistics
{
public:
int frame_rate;
int numberOfParticles;
double frameCalculationInterval;

double dt;
double G;

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


Statistics(double dt, double G, double frameCalculationInterval);

void calculateStatistics(ParticlesState &particles);
void updateStatistics(ParticlesState &particles, int &frameCounter);

void calculateInitialStatistics(ParticlesState &particles);
};