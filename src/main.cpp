#include <raylib.h>
#include "pairwise_algorithm/pairwise.hpp"
#include <iostream>
#include "ui.hpp"


int main()
{
    InitWindow(900, 900, "particlesim2");
    SetTargetFPS(60);
    Pairwise game(4000, 1, 1, 0.1);
    UserInterface userinterface;
    while(WindowShouldClose() == false)
    {
        BeginDrawing();



            ClearBackground(BLACK);
            game.Update(0.1);
            userinterface.Draw();
            DrawFPS(820, 0);




        EndDrawing();
        
   
    }   
}


