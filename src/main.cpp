#include <raylib.h>
#include "pairwise_algorithm/pairwise.hpp"
#include <iostream>
#include <memory>
#include <barneshut_algorithm/barneshut.hpp>
#include "userinterface/ui.hpp"


int main()
{
    int integratorSelectionValue;
    InitWindow(900, 900, "particlesim2");
    SetTargetFPS(60);
    Font calibri = LoadFontEx("src/calibri.ttf", 30, NULL, 0);

    std::unique_ptr<Pairwise> algorithm_pairwise = nullptr;
    std::unique_ptr<BarnesHut> algorithm_barneshut = nullptr;

    UserInterface userinterface(GetFontDefault());
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
            ClearBackground(BLACK);
            Vector2 mousePosition = GetMousePosition();
            userinterface.UpdateButtons(mousePosition);
            if(userinterface.simulationStart)
            {
                if(algorithm_pairwise == nullptr && algorithm_barneshut == nullptr)
                {
                    //Selecting Integrator for Initialisation
                    if(userinterface.EulerButton.buttonPressed)
                    {
                        integratorSelectionValue = 1;
                    }
                    else if(userinterface.VerletButton.buttonPressed)
                    {
                        integratorSelectionValue = 2;
                    }
                    else if(userinterface.RK4Button.buttonPressed)
                    {
                        integratorSelectionValue = 3;
                    }

                    //Selecting Algorithm for Initialisation
                    if(userinterface.BarnesHutButton.buttonPressed)
                    {
                        algorithm_barneshut = std::make_unique<BarnesHut>(15000, integratorSelectionValue, 1, 0.1);
                        algorithm_barneshut->InitialiseParticles();

                    }
                    else if(userinterface.PairwiseButton.buttonPressed)
                    {
                        algorithm_pairwise = std::make_unique<Pairwise>(4000, integratorSelectionValue, 1, 0.1);
                        algorithm_pairwise->InitialiseParticles();
                    }
                }

                if(algorithm_barneshut)
                {
                    algorithm_barneshut->Update();
                }
                else if(algorithm_pairwise)
                {
                    algorithm_pairwise->Update();
                }
                
            }
            else
            {
                userinterface.Draw();
            }
            DrawFPS(820, 0);
        EndDrawing();
        
   
    }   
}


