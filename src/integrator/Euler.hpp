#pragma once
#include "particlestate/ParticlesState.hpp"

class Euler
{
public:
    void Update(ParticlesState &particles, double dt);
};