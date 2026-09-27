#include "button.hpp"

Button::Button(float x, float y, float width, float height, std::string text, Font font, Color buttonColour, bool integrator, bool startButton)
{
    ButtonBounds = Rectangle{x, y, width, height};
    this->text = text;
    this->font = font;
    this->buttonColour = buttonColour;
    this->integrator = integrator;
    this->startButton = startButton;
}

void Button::Draw()
{
    DrawRectangleRec(ButtonBounds, buttonColour);

    Vector2 textDimensions = MeasureTextEx(font, text.c_str(), 30, 1.0f);

    float buttonTextX = ButtonBounds.x + (ButtonBounds.width - textDimensions.x) / 2;

    float buttonTextY = ButtonBounds.y + (ButtonBounds.height - textDimensions.y) / 2;

    DrawTextEx(font,text.c_str(), Vector2{buttonTextX, buttonTextY}, 30.0f, 1.0f, WHITE
    );
}
