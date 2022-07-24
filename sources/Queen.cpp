#include "../headers/Queen.h"
#include "../headers/Board.h"
#include "../headers/HelperFunctions.h"

std::vector<Square *> Queen::moveable_squares(Square *) {
    FDirector fDirector(m_color);
    return HelperFunctions::get_squares_in_fdirections(m_board, m_square, &fDirector,
                                                       {&FDirector::up, &FDirector::down, &FDirector::left,
                                                        &FDirector::right, &FDirector::upleft, &FDirector::upright,
                                                        &FDirector::downleft, &FDirector::downright});
}