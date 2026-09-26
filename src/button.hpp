#pragma once
#include <raylib.h>
#include <string>

class Button
{
public:
Button(float x, float y, float width, float height, std::string text, Font font, Color buttonColour, bool integrator, bool startButton);
Rectangle ButtonBounds;
std::string text;
Font font;
Color buttonColour;
bool buttonPressed = false;
bool integrator;
bool startButton;
void Draw();

};