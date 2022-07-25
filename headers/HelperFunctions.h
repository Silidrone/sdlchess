#ifndef CHESS_HELPERFUNCTIONS_H
#define CHESS_HELPERFUNCTIONS_H

#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Square.h"
#include "Board.h"

namespace HelperFunctions {
    bool squaresHaveAnyPieces(std::vector<Square *>);

    std::pair<std::vector<Square *>, std::vector<DirectionalSquares>>
    get_squares_in_fdirections(Board *, Square *, FDirector *, std::vector<std::pair<Direction, FDirection>>,
                               bool = false);
    std::vector<Square *> get_squares_by_direction(std::vector<DirectionalSquares> &, Direction);
};

#endif //CHESS_HELPERFUNCTIONS_H
