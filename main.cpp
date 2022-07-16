#include <iostream>
#include "headers/Game.h"

int main( int argc, char* args[] )
{
    std::cout << "Chess Game!" << std::endl;
    Game game;
    game.init();
    game.run();
}