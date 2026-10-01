#include "ui.hpp"
#include "button.hpp"
#include <iostream>

UserInterface::UserInterface(Font font) :
    BarnesHutButton(50, 100, 375, 150, "BARNESHUT", font, RED, true, false, false, false),
    PairwiseButton(475, 100, 375, 150, "PAIRWISE", font, RED, true, false, false, false),

    EulerButton(75, 325, 200, 150, "EULER", font, RED, false, true, false, false),
    VerletButton(350, 325, 200, 150, "VERLET", font, RED, false, true, false, false),
    RK4Button(625, 325, 200, 150, "RK4", font, RED, false, true, false, false),

    GalaxyButton(75, 540, 200, 150, "GALAXY", font, RED, false, false, false, true),
    SingleStarButton(350, 540, 200, 150, "SINGLE STAR", font, RED, false, false, false, true),
    BinaryStarButton(625, 540, 200, 150, "BINARY STAR", font, RED, false, false, false, true),

    StartButton(250, 750, 400, 100, "START", font, RED, false, false, true, false),
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

    GalaxyButton.Draw();
    SingleStarButton.Draw();
    BinaryStarButton.Draw();
    
    StartButton.Draw();

    //Titles Text
    DrawText("N BODY PARTICLE SIMULATION", 204, 25, 30, WHITE);
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

    ButtonPressed(mousePosition, GalaxyButton);
    ButtonPressed(mousePosition, SingleStarButton);
    ButtonPressed(mousePosition, BinaryStarButton);


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
        else if (button.integrator)
        {
            SetActiveIntegrator(button);
        }
        else if(button.algorithm)
        {
            SetActiveAlgorithm(button);
        }
        else
        {
            SetActiveParticleConfiguration(button);
        }
    }
}

void UserInterface::SetActiveAlgorithm(Button &button)
{
    //Turns them all of and then makes the one clicked on
    PairwiseButton.buttonPressed = false;
    PairwiseButton.buttonColour = RED;

    BarnesHutButton.buttonPressed = false;
    BarnesHutButton.buttonColour = RED;

    button.buttonPressed = true;
    button.buttonColour = GREEN;
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

void UserInterface::SetActiveParticleConfiguration(Button &button)
{
    //Turns them all of and then makes the one clicked on
    GalaxyButton.buttonPressed = false;
    GalaxyButton.buttonColour = RED;

    SingleStarButton.buttonPressed = false;
    SingleStarButton.buttonColour = RED;

    BinaryStarButton.buttonPressed = false;
    BinaryStarButton.buttonColour = RED;

    button.buttonPressed = true;
    button.buttonColour = GREEN;
}

void UserInterface::SimulationBegin()
{
    if(BarnesHutButton.buttonPressed || PairwiseButton.buttonPressed)
    {
        if(RK4Button.buttonPressed || EulerButton.buttonPressed || VerletButton.buttonPressed)
        {
            if(GalaxyButton.buttonPressed || SingleStarButton.buttonPressed || BinaryStarButton.buttonPressed)
            {
                std::cout << "Begin";
                simulationStart = true;
            }

        }
    }
}