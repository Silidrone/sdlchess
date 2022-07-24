#include "../headers/Knight.h"
#include "../headers/HelperFunctions.h"

std::vector<Square *> Knight::moveable_squares(Square *) {
    FDirector fDirector(m_color);
    return HelperFunctions::get_squares_in_fdirections(
            m_board, m_square, &fDirector,
            {&FDirector::upupleft, &FDirector::upupright, &FDirector::downdownleft, &FDirector::downdownright,
             &FDirector::upleftleft, &FDirector::uprightright, &FDirector::downleftleft, &FDirector::downrightright},
            true
    );
}