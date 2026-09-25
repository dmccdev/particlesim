#include "button.hpp"
#include <iostream>

Button::Button(float x, float y, float width, float height, std::string text, Font font, Color buttonColour, bool integrator)
{
    ButtonBounds = Rectangle{x, y, width, height};
    this->text = text;
    this->font = font;
    this->buttonColour = buttonColour;
    this->integrator = integrator;
}

void Button::Draw()
{
    DrawRectangleRec(ButtonBounds, buttonColour);
    Vector2 textDimensions = MeasureTextEx(font, text.c_str(), 30, 1.0f);
    float BarnesHutButtonX = ButtonBounds.x + (ButtonBounds.width - textDimensions.x) / 2;
    float BarnesHutButtonY = ButtonBounds.y + (ButtonBounds.height - textDimensions.y) / 2;
    DrawTextEx(font, text.c_str(), Vector2{BarnesHutButtonX, BarnesHutButtonY}, 30.0f, 1.0f, WHITE);
}
