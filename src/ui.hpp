#pragma once
#include <raylib.h>
#include <button.hpp>
#include <vector>

class UserInterface
{
public:
    Button BarnesHutButton;
    Button PairwiseButton;
    Button EulerButton;
    Button VerletButton;
    Button RK4Button;

    UserInterface(Font font);
    void Draw();
    void UpdateButtons(Vector2 mousePosition);
    void ButtonPressed(Vector2 mousePosition, Button &button);
    void SetActiveIntegrator(Button &button);

private:
};