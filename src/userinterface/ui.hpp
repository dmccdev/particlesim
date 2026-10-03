#pragma once
#include <raylib.h>
#include "button.hpp"
#include "statistics/statistics.hpp"

class UserInterface
{
public:
    Font font;
    Button BarnesHutButton;
    Button PairwiseButton;

    Button EulerButton;
    Button VerletButton;
    Button RK4Button;
    Button StartButton;
    Button GalaxyButton;
    Button SingleStarButton;
    Button BinaryStarButton;

    bool simulationStart;

    UserInterface(Font font, double dt, double G);

    void Draw();
    void UpdateButtons(Vector2 mousePosition);
    void ButtonPressed(Vector2 mousePosition, Button &button);

    void SetActiveParticleConfiguration(Button &button);
    void SetActiveIntegrator(Button &button);
    void SetActiveAlgorithm(Button &button);
    void SimulationBegin();

    void DrawStatistics(Statistics &Stats);

private:
};
