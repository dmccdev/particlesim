#include "ui.hpp"
#include "button.hpp"
#include <iostream>

UserInterface::UserInterface(Font font, double dt, double G) :
    BarnesHutButton(50, 100, 525, 150, "BARNESHUT", RED, true, false, false, false),
    PairwiseButton(625, 100, 525, 150, "PAIRWISE", RED, true, false, false, false),

    EulerButton(75, 325, 300, 150, "EULER", RED, false, true, false, false),
    VerletButton(450, 325, 300, 150, "VERLET", RED, false, true, false, false),
    RK4Button(825, 325, 300, 150, "RK4", RED, false, true, false, false),

    GalaxyButton(75, 540, 300, 150, "GALAXY", RED, false, false, false, true),
    SingleStarButton(450, 540, 300, 150, "SINGLE STAR", RED, false, false, false, true),
    BinaryStarButton(825, 540, 300, 150, "BINARY STAR", RED, false, false, false, true),

    StartButton(400, 750, 400, 100, "START", RED, false, false, true, false),
    simulationStart(false),
    font(font)
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

    // Titles Text
    DrawText("N BODY PARTICLE SIMULATION", 354, 25, 30, WHITE);
    DrawText("ALGORITHM", 545, 70, 20, WHITE);
    DrawText("INTEGRATOR", 535, 285, 20, WHITE);
    DrawText("PARTICLE CONFIGURATION", 465, 500, 20, WHITE);

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

void UserInterface::DrawStatistics(Statistics &Stats)
{
    int panelX = 900;
    int panelWidth = 300;
    int panelHeight = 900;

    DrawRectangle(panelX, 0, panelWidth, panelHeight, DARKGRAY);

    DrawTextEx(font, "STATISTICS", { panelX + 20.0f, 25.0f }, 24.0f, 1.0f, WHITE);

    // Energy
    DrawTextEx(font, "ENERGY", { panelX + 20.0f, 90.0f }, 18.0f, 1.0f, WHITE);
    DrawLine(panelX + 20, 118, panelX + 280, 118, GRAY);

    DrawTextEx(font, TextFormat("Kinetic:  %.4e", Stats.totalKineticEnergy), { panelX + 20.0f, 140.0f }, 16.0f, 1.0f, LIGHTGRAY);
    DrawTextEx(font, TextFormat("Potential: %.4e", Stats.totalPotentialEnergy), { panelX + 20.0f, 168.0f }, 16.0f, 1.0f, LIGHTGRAY);
    DrawTextEx(font, TextFormat("Total:     %.4e", Stats.totalEnergy), { panelX + 20.0f, 196.0f }, 16.0f, 1.0f, LIGHTGRAY);
    DrawTextEx(font, TextFormat("Initial:   %.4e", Stats.initialTotalEnergy), { panelX + 20.0f, 224.0f }, 16.0f, 1.0f, LIGHTGRAY);
    DrawTextEx(font, TextFormat("Error:     %.4f%%", Stats.energyError), { panelX + 20.0f, 252.0f }, 16.0f, 1.0f, LIGHTGRAY);

    // Momentum
    DrawTextEx(font, "MOMENTUM", { panelX + 20.0f, 315.0f }, 18.0f, 1.0f, WHITE);
    DrawLine(panelX + 20, 343, panelX + 280, 343, GRAY);

    DrawTextEx(font, TextFormat("Px:         %.4e", Stats.linearMomentumX), { panelX + 20.0f, 365.0f }, 16.0f, 1.0f, LIGHTGRAY);
    DrawTextEx(font, TextFormat("Py:         %.4e", Stats.linearMomentumY), { panelX + 20.0f, 393.0f }, 16.0f, 1.0f, LIGHTGRAY);
    DrawTextEx(font, TextFormat("Initial Px: %.4e", Stats.initialLinearMomentumX), { panelX + 20.0f, 421.0f }, 16.0f, 1.0f, LIGHTGRAY);
    DrawTextEx(font, TextFormat("Initial Py: %.4e", Stats.initialLinearMomentumY), { panelX + 20.0f, 449.0f }, 16.0f, 1.0f, LIGHTGRAY);
    DrawTextEx(font, TextFormat("Px Error:   %.4e", Stats.linearMomentumErrorX), { panelX + 20.0f, 477.0f }, 16.0f, 1.0f, LIGHTGRAY);
    DrawTextEx(font, TextFormat("Py Error:   %.4e", Stats.linearMomentumErrorY), { panelX + 20.0f, 505.0f }, 16.0f, 1.0f, LIGHTGRAY);
}

