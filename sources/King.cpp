#include "../headers/King.h"
#include "../headers/Square.h"

std::vector<Square *> King::attacked_squares() {
    return m_board->get_squares_in_fdirections(
            this,
            {
                    {Direction::UP,         &FDirector::up},
                    {Direction::DOWN,       &FDirector::down},
                    {Direction::LEFT,       &FDirector::left},
                    {Direction::RIGHT,      &FDirector::right},
                    {Direction::UP_LEFT,    &FDirector::up_left},
                    {Direction::UP_RIGHT,   &FDirector::up_right},
                    {Direction::DOWN_LEFT,  &FDirector::down_left},
                    {Direction::DOWN_RIGHT, &FDirector::down_right},
            }, true).first;
}

bool King::can_move_to_attacked(Square *s) {
    return !s->isAttacked(static_cast<ChessColor>(!static_cast<bool>(getColor())));
}