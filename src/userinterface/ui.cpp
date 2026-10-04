#include "ui.hpp"
#include "button.hpp"
#include <iostream>
#include <cmath>

UserInterface::UserInterface(Font font, double dt, double G) :
    BarnesHutButton(50, 100, 525, 150, "BARNESHUT", RED, true, false, false, false),
    PairwiseButton(625, 100, 525, 150, "PAIRWISE", RED, true, false, false, false),

    EulerButton(75, 325, 300, 150, "EULER", RED, false, true, false, false),
    VerletButton(450, 325, 300, 150, "VERLET", RED, false, true, false, false),
    RK4Button(825, 325, 300, 150, "RK4", RED, false, true, false, false),

    GalaxyButton(75, 540, 300, 150, "GALAXY", RED, false, false, false, true),
    SingleStarButton(450, 540, 300, 150, "SINGLE STAR", RED, false, false, false, true),
    BinaryStarButton(825, 540, 300, 150, "BINARY STAR", RED, false, false, false, true),

    StatisticsButton(400, 725, 400, 75, "STATISTICS", RED, false, false, false, false),

    StartButton(400, 825, 400, 75, "START", RED, false, false, true, false),

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

    StatisticsButton.Draw();
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

    ButtonPressed(mousePosition, StatisticsButton);

    ButtonPressed(mousePosition, StartButton);
}

void UserInterface::ButtonPressed(Vector2 mousePosition, Button &button)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(mousePosition, button.ButtonBounds))
    {
        if (button.startButton)
        {
            SimulationBegin();
            return;
        }
        else if (&button == &StatisticsButton)
        {
            ToggleStatistics();
            return;
        }
        else if (button.integrator)
        {
            SetActiveIntegrator(button);
        }
        else if (button.algorithm)
        {
            SetActiveAlgorithm(button);
        }
        else
        {
            SetActiveParticleConfiguration(button);
        }
    }
}

void UserInterface::ToggleStatistics()
{
    //Flip boolean state of statistics button
    StatisticsButton.buttonPressed = !StatisticsButton.buttonPressed;

    if (StatisticsButton.buttonPressed)
    {
        StatisticsButton.buttonColour = GREEN;
    }
    else
    {
        StatisticsButton.buttonColour = RED;
    }
}

void UserInterface::SetActiveAlgorithm(Button &button)
{
    // Turns them all off and then makes the one clicked on
    PairwiseButton.buttonPressed = false;
    PairwiseButton.buttonColour = RED;

    BarnesHutButton.buttonPressed = false;
    BarnesHutButton.buttonColour = RED;

    button.buttonPressed = true;
    button.buttonColour = GREEN;
}

void UserInterface::SetActiveIntegrator(Button &button)
{
    // Turns them all off and then makes the one clicked on
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
    // Turns them all off and then makes the one clicked on
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
    if (BarnesHutButton.buttonPressed || PairwiseButton.buttonPressed)
    {
        if (RK4Button.buttonPressed || EulerButton.buttonPressed || VerletButton.buttonPressed)
        {
            if (GalaxyButton.buttonPressed || SingleStarButton.buttonPressed || BinaryStarButton.buttonPressed)
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

    // Title
    DrawText("STATISTICS", panelX + 20, 25, 24, WHITE);

    // Energy
    DrawText("ENERGY", panelX + 20, 90, 18, WHITE);
    DrawLine(panelX + 20, 118, panelX + 280, 118, GRAY);

    DrawText(TextFormat("Kinetic:  %.2f", Stats.totalKineticEnergy), panelX + 20, 140, 16, LIGHTGRAY);
    DrawText(TextFormat("Potential: %.2f", Stats.totalPotentialEnergy), panelX + 20, 168, 16, LIGHTGRAY);
    DrawText(TextFormat("Total:     %.2f", Stats.totalEnergy), panelX + 20, 196, 16, LIGHTGRAY);
    DrawText(TextFormat("Initial:   %.2f", Stats.initialTotalEnergy), panelX + 20, 224, 16, LIGHTGRAY);
    DrawText(TextFormat("Error:     %.4f%%", Stats.energyError), panelX + 20, 252, 16, LIGHTGRAY);

    // Momentum
    DrawText("MOMENTUM", panelX + 20, 315, 18, WHITE);
    DrawLine(panelX + 20, 343, panelX + 280, 343, GRAY);

    DrawText(TextFormat("Px:         %.2f", Stats.linearMomentumX), panelX + 20, 365, 16, LIGHTGRAY);
    DrawText(TextFormat("Py:         %.2f", Stats.linearMomentumY), panelX + 20, 393, 16, LIGHTGRAY);
    DrawText(TextFormat("Initial Px: %.2f", Stats.initialLinearMomentumX), panelX + 20, 421, 16, LIGHTGRAY);
    DrawText(TextFormat("Initial Py: %.2f", Stats.initialLinearMomentumY), panelX + 20, 449, 16, LIGHTGRAY);
    DrawText(TextFormat("Px Error:   %.4f%%", Stats.linearMomentumErrorX), panelX + 20, 477, 16, LIGHTGRAY);
    DrawText(TextFormat("Py Error:   %.4f%%", Stats.linearMomentumErrorY), panelX + 20, 505, 16, LIGHTGRAY);
}

