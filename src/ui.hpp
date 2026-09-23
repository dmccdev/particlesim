#pragma once
#include <raylib.h>

class UserInterface
{
public:
    Rectangle BarnesHutButton;
    Rectangle PairwiseButton;

    Rectangle EulerButton;
    Rectangle VerletButton;
    Rectangle RK4Button;

    UserInterface();
    void Draw();
    bool ButtonPressed(Vector2 mousePosition, Rectangle &button);

private:
};