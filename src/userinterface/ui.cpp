#include "ui.hpp"
#include "button.hpp"
#include <iostream>

UserInterface::UserInterface(Font font, double dt, double G) :
    BarnesHutButton(50, 100, 375, 150, "BARNESHUT", font, RED, true, false, false, false),
    PairwiseButton(475, 100, 375, 150, "PAIRWISE", font, RED, true, false, false, false),

    EulerButton(75, 325, 200, 150, "EULER", font, RED, false, true, false, false),
    VerletButton(350, 325, 200, 150, "VERLET", font, RED, false, true, false, false),
    RK4Button(625, 325, 200, 150, "RK4", font, RED, false, true, false, false),

    GalaxyButton(75, 540, 200, 150, "GALAXY", font, RED, false, false, false, true),
    SingleStarButton(350, 540, 200, 150, "SINGLE STAR", font, RED, false, false, false, true),
    BinaryStarButton(625, 540, 200, 150, "BINARY STAR", font, RED, false, false, false, true),

    StartButton(250, 750, 400, 100, "START", font, RED, false, false, true, false),
    Stats(dt, G, 300),
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

void UserInterface::DrawStatistics()
{
    int panelX = 900;
    int panelWidth = 300;
    int panelHeight = 900;

    //Background

    DrawRectangle(panelX, 0, panelWidth, panelHeight, DARKGRAY);

    //Title

    DrawText("STATISTICS", panelX + 75, 25, 25, WHITE);

    //Performance Stats

    DrawText("PERFORMANCE", panelX + 20, 80, 20, WHITE);

    DrawText(TextFormat("FPS: %d", Stats.frame_rate), panelX + 20, 115, 18, LIGHTGRAY);

    DrawText(TextFormat("Particles: %d", Stats.numberOfParticles),panelX + 20, 145, 18, LIGHTGRAY);

    //Energy

    DrawText("ENERGY", panelX + 20, 200, 20, WHITE);
    DrawText(TextFormat("Kinetic: %.4e", Stats.totalKineticEnergy),panelX + 20, 235, 17, LIGHTGRAY);
    DrawText(TextFormat("Potential: %.4e", Stats.totalPotentialEnergy), panelX + 20, 265, 17, LIGHTGRAY);
    DrawText(TextFormat("Total: %.4e", Stats.totalKineticEnergy + Stats.totalPotentialEnergy), panelX + 20, 295, 17, LIGHTGRAY);
    DrawText(TextFormat("Energy Error: %.4f%%", Stats.energyError), panelX + 20, 325, 17, LIGHTGRAY);

    //Linear Momentum 

    DrawText("LINEAR MOMENTUM", panelX + 20, 380, 20, WHITE);
    DrawText(TextFormat("Px: %.4e", Stats.linearMomentumX), panelX + 20, 415, 17, LIGHTGRAY);
    DrawText(TextFormat("Py: %.4e", Stats.linearMomentumY), panelX + 20, 445, 17, LIGHTGRAY);
    DrawText(TextFormat("Px Error: %.4e", Stats.linearMomentumErrorX), panelX + 20, 475, 17, LIGHTGRAY);
    DrawText(TextFormat("Py Error: %.4e", Stats.linearMomentumErrorY), panelX + 20, 505, 17, LIGHTGRAY);

    //Angular Momentum

    DrawText("ANGULAR MOMENTUM", panelX + 20, 560, 20, WHITE);
    DrawText(TextFormat("L: %.4e", Stats.angularMomentum), panelX + 20, 595, 17, LIGHTGRAY);
    DrawText(TextFormat("L Error: %.4e", Stats.angularMomentumError), panelX + 20, 625, 17, LIGHTGRAY);

    //Calculation Infomation

    DrawText("UPDATE", panelX + 20, 680, 20, WHITE);
    DrawText(TextFormat("Every %.0f frames", Stats.frameCalculationInterval), panelX + 20, 715, 17, LIGHTGRAY);
    DrawText(TextFormat("dt: %.6f", Stats.dt), panelX + 20, 745, 17, LIGHTGRAY);
    DrawText(TextFormat("G: %.6e", Stats.G),panelX + 20, 775, 17, LIGHTGRAY);
}