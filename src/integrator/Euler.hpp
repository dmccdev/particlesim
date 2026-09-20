#pragma once
#include "Integrator.hpp"



class Euler: public Integrator
{
public:
    void Update(ParticlesState &particles, double dt);
};