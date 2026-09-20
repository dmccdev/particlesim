#include <raylib.h>
#include "pairwise_algorithm/pairwise.hpp"
#include <iostream>


int main()
{
    InitWindow(900, 900, "particlesim2");
    SetTargetFPS(60);
    Pairwise game(2000, 2, 2, 0.01);
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
            ClearBackground(BLACK);
            game.Update(0.01);
            DrawFPS(820, 0);
        EndDrawing();
        
   
    }   
}


