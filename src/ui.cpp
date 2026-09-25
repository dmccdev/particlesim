#include "ui.hpp"
#include <string>
#include <button.hpp>


UserInterface::UserInterface(Font font) :
BarnesHutButton(50, 75, 375, 200, "BARNES-HUT", font, RED, false),
PairwiseButton(475, 75, 375, 200, "PAIRWISE", font, RED, false),
EulerButton(75, 400, 200, 200, "EULER", font, RED, true),
VerletButton(350, 400, 200, 200, "VERLET", font, RED, true),
RK4Button(625, 400, 200, 200, "RK4", font, RED, true)
{
}

void UserInterface::Draw()
{

BarnesHutButton.Draw();
PairwiseButton.Draw();
EulerButton.Draw();
VerletButton.Draw();
RK4Button.Draw();

}

void UserInterface::UpdateButtons(Vector2 mousePosition)
{
    ButtonPressed(mousePosition, BarnesHutButton);
    ButtonPressed(mousePosition, PairwiseButton);
    ButtonPressed(mousePosition, EulerButton);
    ButtonPressed(mousePosition, VerletButton);
    ButtonPressed(mousePosition, RK4Button);
}

void UserInterface::ButtonPressed(Vector2 mousePosition, Button &button)
{
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, button.ButtonBounds))
    {
        SetActiveIntegrator(button);
    }
}

void UserInterface::SetActiveIntegrator(Button &button)
{
    if( (RK4Button.buttonPressed + EulerButton.buttonPressed + RK4Button.buttonPressed) == 0 && button.integrator == true)
        {
            button.buttonColour = GREEN;
            button.buttonPressed = true;
        }
}
