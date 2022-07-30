#include "../headers/Knight.h"

std::vector<Square *> Knight::moveable_squares(Square *) {
    return m_board->get_squares_in_fdirections(
            m_square,
            {
                    {Direction::UP_UP_LEFT,       &FDirector::up_up_left},
                    {Direction::UP_UP_RIGHT,      &FDirector::up_up_right},
                    {Direction::DOWN_DOWN_LEFT,   &FDirector::down_down_left},
                    {Direction::DOWN_DOWN_RIGHT,  &FDirector::down_down_right},
                    {Direction::UP_LEFT_LEFT,     &FDirector::up_left_left},
                    {Direction::UP_RIGHT_RIGHT,   &FDirector::up_right_right},
                    {Direction::DOWN_LEFT_LEFT,   &FDirector::down_left_left},
                    {Direction::DOWN_RIGHT_RIGHT, &FDirector::down_right_right},
            }, true).first;
}
