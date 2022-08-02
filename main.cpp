#include <iostream>
#include "headers/Game.h"

int main(int argc, char *args[]) {
    std::cout << "main" << std::endl;
    Game::instance().init();
    Game::instance().run();
}