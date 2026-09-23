#include "ui.hpp"
#include <string>
#include <button.hpp>

UserInterface::UserInterface() 
{
}

void UserInterface::Draw()
{

Font calibri = LoadFontEx("src/calibri.ttf", 30, NULL, 0);

Button BarnesHutButton(50, 75, 375, 200, "BARNES-HUT", calibri, RED);
BarnesHutButton.Draw();

Button PairwiseButton(475, 75, 375, 200, "PAIRWISE", calibri, RED);
PairwiseButton.Draw();

Button EulerButton(75, 400, 200, 200, "EULER", calibri, RED);
EulerButton.Draw();

Button VerletButton(350, 400, 200, 200, "VERLET", calibri, RED);
VerletButton.Draw();

Button RK4Button(625, 400, 200, 200, "RK4", calibri, RED);
RK4Button.Draw();


}


// bool UserInterface::ButtonPressed(Vector2 mousePosition, Rectangle &button)
// {
//     if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, RK4Button))
//     {
//     }
// }
