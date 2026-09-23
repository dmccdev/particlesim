#pragma once
#include <raylib.h>
#include <string>

class Button
{
public:
Button(float x, float y, float width, float height, std::string text, Font font, Color buttonColour);
Rectangle ButtonBounds;
std::string text;
Font font;
Color buttonColour;
bool buttonPressed;
void Draw();

};