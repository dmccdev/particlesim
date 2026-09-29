#include "ui.hpp"
#include "button.hpp"
#include <iostream>

UserInterface::UserInterface(Font font) :
    BarnesHutButton(50, 100, 375, 150, "BARNESHUT", font, RED, false, false),
    PairwiseButton(475, 100, 375, 150, "PAIRWISE", font, RED, false, false),

    EulerButton(75, 325, 200, 150, "EULER", font, RED, true, false),
    VerletButton(350, 325, 200, 150, "VERLET", font, RED, true, false),
    RK4Button(625, 325, 200, 150, "RK4", font, RED, true, false),

    TriangleButton(75, 540, 200, 150, "TRIANGLE", font, RED, false, false),
    GalaxyButton(350, 540, 200, 150, "GALAXY", font, RED, false, false),
    BinaryGalaxyButton(625, 540, 200, 150, "DUAL GALAXY", font, RED, false, false),

    StartButton(250, 750, 400, 100, "START", font, RED, false, true),
    simulationStart(false)
{
}

void UserInterface::Draw()
{
    BarnesHutButton.Draw();
    PairwiseButton.Draw();

    EulerButton.Draw();
    VerletButton.Draw();
    RK4Button.Draw();

    TriangleButton.Draw();
    GalaxyButton.Draw();
    BinaryGalaxyButton.Draw();
    
    StartButton.Draw();

    //Titles
    DrawText("PARTICLE SIM", 350, 25, 30, WHITE);
    DrawText("ALGORITHM", 395, 70, 20, WHITE);
    DrawText("INTEGRATOR", 385, 285, 20, WHITE);
    DrawText("PARTICLE CONFIGURATION", 315, 500, 20, WHITE);

}

void UserInterface::UpdateButtons(Vector2 mousePosition)
{
    ButtonPressed(mousePosition, BarnesHutButton);
    ButtonPressed(mousePosition, PairwiseButton);

    ButtonPressed(mousePosition, EulerButton);
    ButtonPressed(mousePosition, VerletButton);
    ButtonPressed(mousePosition, RK4Button);

    ButtonPressed(mousePosition, StartButton);
}

void UserInterface::ButtonPressed(Vector2 mousePosition, Button &button)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(mousePosition, button.ButtonBounds))
    {
        if(button.startButton)
        {
            SimulationBegin();
            return;
        }


        if (button.integrator)
        {
            SetActiveIntegrator(button);
        }
        else
        {
            SetActiveAlgorithm(button);
        }
    }
}

void UserInterface::SetActiveIntegrator(Button &button)
{
    //Turns them all of and then makes the one clicked on
    EulerButton.buttonPressed = false;
    EulerButton.buttonColour = RED;

    VerletButton.buttonPressed = false;
    VerletButton.buttonColour = RED;

    RK4Button.buttonPressed = false;
    RK4Button.buttonColour = RED;

    button.buttonPressed = true;
    button.buttonColour = GREEN;
}

void UserInterface::SetActiveAlgorithm(Button &button)
{
    //Turns them all of and then makes the one clicked on
    BarnesHutButton.buttonPressed = false;
    BarnesHutButton.buttonColour = RED;

    PairwiseButton.buttonPressed = false;
    PairwiseButton.buttonColour = RED;

    button.buttonPressed = true;
    button.buttonColour = GREEN;
}

void UserInterface::SimulationBegin()
{
    if(BarnesHutButton.buttonPressed || PairwiseButton.buttonPressed)
    {
        if(RK4Button.buttonPressed || EulerButton.buttonPressed || VerletButton.buttonPressed)
        {
            std::cout << "Begin";
            simulationStart = true;
        }
    }
}