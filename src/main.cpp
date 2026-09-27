#include <raylib.h>
#include "pairwise_algorithm/pairwise.hpp"
#include <iostream>
#include <memory>
#include "userinterface/ui.hpp"


int main()
{
    int integratorSelectionValue;
    InitWindow(900, 900, "particlesim2");
    SetTargetFPS(60);
    Font calibri = LoadFontEx("src/calibri.ttf", 30, NULL, 0);

    std::unique_ptr<Pairwise> algorithm = nullptr;

    UserInterface userinterface(calibri);
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
            ClearBackground(BLACK);
            if(userinterface.simulationStart)
            {
                if(algorithm == nullptr)
                {
                    //Deciding initialisation values

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

                    algorithm = std::make_unique<Pairwise>(4000, integratorSelectionValue, 2, 0.1);

                }

                algorithm->Update();
            }
            else
            {
                userinterface.Draw();
            }

            Vector2 mousePosition = GetMousePosition();
            userinterface.UpdateButtons(mousePosition);
            DrawFPS(820, 0);
        EndDrawing();
        
   
    }   
}


