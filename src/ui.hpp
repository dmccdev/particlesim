#pragma once
#include <raylib.h>

class UserInterface
{
public:
    UserInterface();
    void Draw();
    bool ButtonPressed(Vector2 mousePosition, Rectangle &button);

private:
};