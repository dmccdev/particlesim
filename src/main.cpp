#include <raylib.h>
#include "pairwise_algorithm/pairwise.hpp"
#include <iostream>
#include "ui.hpp"


int main()
{

    InitWindow(900, 900, "particlesim2");
    SetTargetFPS(60);
    Font calibri = LoadFontEx("src/calibri.ttf", 30, NULL, 0);
    Pairwise game(4000, 1, 1, 0.1);
    UserInterface userinterface(calibri);
    while(WindowShouldClose() == false)
    {
        BeginDrawing();



            ClearBackground(BLACK);
            game.Update(0.1);
            userinterface.Draw();
            Vector2 mousePosition = GetMousePosition();
            userinterface.UpdateButtons(mousePosition);
            DrawFPS(820, 0);
        EndDrawing();
        
   
    }   
}


