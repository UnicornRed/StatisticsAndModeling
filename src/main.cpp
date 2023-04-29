#include <iostream>
#include "Program_NB.h"

int main(int argn, char **argv)
{
    seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator = std::default_random_engine(seed);

    program_NB p;

    return p.run();
}