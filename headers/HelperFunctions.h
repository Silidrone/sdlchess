#ifndef CHESS_HELPERFUNCTIONS_H
#define CHESS_HELPERFUNCTIONS_H

#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Square.h"
#include "Board.h"

namespace HelperFunctions {
    bool squaresHaveAnyPieces(std::vector<Square *> squares);

    std::vector<Square *>
    get_squares_in_fdirections(Board *board, Square *beginning_square, FDirector *, std::vector<FDirection> directions,
                               bool one_step = false);
};

#endif //CHESS_HELPERFUNCTIONS_H
