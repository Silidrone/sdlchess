#include "../headers/Rook.h"
#include <vector>
#include "../headers/Board.h"
#include <algorithm>
#include "../headers/HelperFunctions.h"

std::vector<Square *> Rook::moveable_squares(Square *) {
    FDirector fDirector(m_color);
    return HelperFunctions::get_squares_in_fdirections(m_board, m_square, &fDirector,
                                                       {&FDirector::up, &FDirector::down});
}