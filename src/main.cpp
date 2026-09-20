#include <raylib.h>
#include "pairwise_algorithm/pairwise.hpp"
#include <iostream>


int main()
{
    InitWindow(900, 900, "particlesim2");
    SetTargetFPS(60);
    PairwiseAlgorithm game(4000, 2, 3, 0.01);
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
            ClearBackground(BLACK);
            game.Update(0.01);
            DrawFPS(820, 0);
        EndDrawing();
        
   
    }   
}


