#include "ui.hpp"

UserInterface::UserInterface() :
BarnesHutButton{50, 50, 375, 200},
PairwiseButton{475, 50, 375, 200},
EulerButton{100, 400, 100, 100},
VerletButton{400, 400, 100, 100},
RK4Button{700, 400, 100, 100}



{
}

void UserInterface::Draw()
{
DrawRectangleRec(BarnesHutButton, GRAY);
DrawRectangleRec(PairwiseButton, GRAY);
DrawRectangleRec(EulerButton, GRAY);
DrawRectangleRec(VerletButton, GRAY);
DrawRectangleRec(RK4Button, GRAY);
}

