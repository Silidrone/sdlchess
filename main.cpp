#include <iostream>
#include "headers/Game.h"

int main(int argc, char *args[]) {
    std::cout << "main" << std::endl;
    Game game;
    game.init();
    game.run();
}