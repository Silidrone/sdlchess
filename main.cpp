#include <iostream>
#include "headers/Game.h"
#include "headers/HelperFunctions.h"

int main(int argc, char *args[]) {
    std::cout << "main.cpp: start chess" << std::endl;
    Game g;
    g.init();
    g.run();
}