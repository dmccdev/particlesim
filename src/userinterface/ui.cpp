#include "ui.hpp"
#include "button.hpp"
#include <iostream>

UserInterface::UserInterface(Font font) :
    BarnesHutButton(50, 75, 375, 200, "BARNES-HUT", font, RED, false, false),
    PairwiseButton(475, 75, 375, 200, "PAIRWISE", font, RED, false, false),

    EulerButton(75, 400, 200, 200, "EULER", font, RED, true, false),
    VerletButton(350, 400, 200, 200, "VERLET", font, RED, true, false),
    RK4Button(625, 400, 200, 200, "RK4", font, RED, true, false),
    StartButton(250, 675, 400, 175, "Start", font, RED, false, true),
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
    
    StartButton.Draw();
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