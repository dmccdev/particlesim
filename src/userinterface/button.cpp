#include "button.hpp"

Button::Button(float x, float y, float width, float height, std::string text, Color buttonColour, bool algorithmButton, bool integrator, bool startButton, bool particleConfigButton)
{
    ButtonBounds = Rectangle{x, y, width, height};
    this->text = text;
    this->buttonColour = buttonColour;
    this->integrator = integrator;
    this->startButton = startButton;
    this->particleConfig = particleConfigButton;
    this->algorithm = algorithmButton;
    
}

void Button::Draw()
{
    DrawRectangleRec(ButtonBounds, buttonColour);

    Vector2 textDimensions = MeasureTextEx(GetFontDefault(), text.c_str(), 30, 1.0f);

    float buttonTextX = ButtonBounds.x + (ButtonBounds.width - textDimensions.x) / 2;
    float buttonTextY = ButtonBounds.y + (ButtonBounds.height - textDimensions.y) / 2;

    DrawTextEx(GetFontDefault() ,text.c_str(), Vector2{buttonTextX, buttonTextY}, 30.0f, 1.0f, WHITE);
}
