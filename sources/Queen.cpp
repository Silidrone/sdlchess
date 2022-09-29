#include "../headers/Queen.h"
#include "../headers/HelperFunctions.h"
#include "../headers/Square.h"

Queen::Queen(ChessColor c, Square *square, Board *b) :
        QRBPiece(c, square, b, MTexture(SharedData::instance().getRenderer(), c == ChessColor::WHITE
                                                                              ? "../resources/w_queen.png"
                                                                              : "../resources/b_queen.png")) {
}

std::vector<std::pair<Direction, FDirection>> Queen::getDirections() {
    return {
            {Direction::UP,         &FDirector::up},
            {Direction::DOWN,       &FDirector::down},
            {Direction::LEFT,       &FDirector::left},
            {Direction::RIGHT,      &FDirector::right},
            {Direction::UP_LEFT,    &FDirector::up_left},
            {Direction::UP_RIGHT,   &FDirector::up_right},
            {Direction::DOWN_LEFT,  &FDirector::down_left},
            {Direction::DOWN_RIGHT, &FDirector::down_right},
    };
}

std::string Queen::move_log(Square *, bool captured) {
    return HelperFunctions::get_algebraic_notation('Q', m_square->getCoordinate(), captured);
}