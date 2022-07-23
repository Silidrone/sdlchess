#include "../headers/Bishop.h"
#include "../headers/HelperFunctions.h"

std::vector<Square *> Bishop::moveable_squares(Square *) {
    return HelperFunctions::get_squares_in_fdirections(m_board, m_square,
                                                       {FDirections::upleft, FDirections::upright,
                                                        FDirections::downleft, FDirections::downright});
}