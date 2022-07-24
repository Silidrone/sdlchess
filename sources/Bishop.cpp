#include "../headers/Bishop.h"
#include "../headers/HelperFunctions.h"

std::vector<Square *> Bishop::moveable_squares(Square *) {
    FDirector fDirector(m_color);
    return HelperFunctions::get_squares_in_fdirections(m_board, m_square, &fDirector,
                                                       {&FDirector::upleft, &FDirector::upright,
                                                        &FDirector::downleft, &FDirector::downright});
}