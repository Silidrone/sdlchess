#ifndef CHESS_HELPERFUNCTIONS_H
#define CHESS_HELPERFUNCTIONS_H

#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Square.h"

namespace HelperFunctions {
    bool sqauresHaveAnyPieces(std::vector<Square *> squares) {
        for(auto &square: squares) {
            if(square->getPiece() != nullptr) return true;
        }

        return false;
    }
};

#endif //CHESS_HELPERFUNCTIONS_H
