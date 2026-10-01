#pragma once
#include <raylib.h>
#include <string>

class Button
{
public:
Button(float x, float y, float width, float height, std::string text, Font font, Color buttonColour,bool algorithmButton, bool integratorButton, bool startButton, bool particleConfigButton);
Rectangle ButtonBounds;
std::string text;
Font font;
Color buttonColour;
bool buttonPressed = false;
bool particleConfig;
bool algorithm;
bool integrator;
bool startButton;
void Draw();

};