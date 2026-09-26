#pragma once
#include <raylib.h>
#include <button.hpp>

class UserInterface
{
public:
    Button BarnesHutButton;
    Button PairwiseButton;

    Button EulerButton;
    Button VerletButton;
    Button RK4Button;
    
    Button StartButton;

    bool simulationStart;

    UserInterface(Font font);

    void Draw();
    void UpdateButtons(Vector2 mousePosition);
    void ButtonPressed(Vector2 mousePosition, Button &button);

    void SetActiveIntegrator(Button &button);
    void SetActiveAlgorithm(Button &button);
    void SimulationBegin();

private:
};
