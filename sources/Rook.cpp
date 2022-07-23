#include "../headers/Rook.h"
#include <vector>
#include "../headers/Board.h"
#include <algorithm>
#include "../headers/HelperFunctions.h"

std::vector<Square *> Rook::moveable_squares(Square *) {
    return HelperFunctions::get_squares_in_fdirections(m_board, m_square,
                                                       {FDirections::up, FDirections::down});
}