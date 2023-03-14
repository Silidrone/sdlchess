#include "../headers/Knight.h"
#include "../headers/HelperFunctions.h"
#include "../headers/Square.h"

Knight::Knight(ChessColor c, Board *b, Square *square) : Piece(c, b,
                                                               MTexture(SharedData::instance().getRenderer(),
                                                                        c == ChessColor::WHITE
                                                                        ? "../resources/w_knight.png"
                                                                        : "../resources/b_knight.png"), square) {}

std::vector<Square *> Knight::attacked_squares() {
    return m_board->get_squares_in_fdirections(
            this,
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

bool Knight::can_move_to_attacked(Square *) { return true; }

std::string Knight::move_log(Square *, bool captured) {
    return HelperFunctions::get_algebraic_notation('N', m_square->getCoordinate(), captured);
}
