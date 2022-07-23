#include "../headers/Queen.h"
#include "../headers/Board.h"
#include "../headers/HelperFunctions.h"

std::vector<Square *> Queen::moveable_squares(Square *) {
    return HelperFunctions::get_squares_in_fdirections(m_board, m_square,
                                                       {FDirections::up, FDirections::down, FDirections::left,
                                                        FDirections::right, FDirections::upleft, FDirections::upright,
                                                        FDirections::downleft, FDirections::downright});
}