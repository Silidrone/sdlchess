#include <iostream>
#include "headers/Game.h"
#include "headers/HelperFunctions.h"

int main(int argc, char *args[]) {
    std::cout << "tester.cpp: start test" << std::endl;
    auto games = HelperFunctions::parsePGN("../pgn_games/Adams.pgn");
    for (int i = 0; i < games.size(); i++) {
        std::cout << "Game " << i + 1 << std::endl;
        int k = 1;
        for (int j = 0; j < games[i].getMoveCount(); j++) {
            if (j % 2 == 0) {
                std::cout << std::endl;
                std::cout << k << ".: ";
                k++;
            }
            std::cout << games[i].getMove(j) << " ";
        }
        std::cout << std::endl << std::endl;
    }
}