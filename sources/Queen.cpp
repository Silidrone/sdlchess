#include "../headers/Queen.h"
#include "../headers/HelperFunctions.h"
#include "../headers/Square.h"

Queen::Queen(ChessColor c, Board *b, Square *square) :
        QRBPiece(c, b, MTexture(SharedData::instance().getRenderer(), c == ChessColor::WHITE
                                                                              ? (SharedData::instance().getResourcesPath() + "w_queen.png").c_str()
                                                                              : (SharedData::instance().getResourcesPath() + "b_queen.png").c_str()), square) {
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