#pragma once

#include "Doc_NB.h"
#include "Draw_NB.h"

class program_NB
{
private:
    Doc_NB data;
    Draw_NB painter;
public:
    program_NB() : data(), painter(&data) { }

    int run();
};