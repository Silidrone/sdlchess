#include "../headers/Bishop.h"
#include "../headers/HelperFunctions.h"
#include "../headers/Square.h"

Bishop::Bishop(ChessColor c, Board *b, Square *square) :
        QRBPiece(c, b,
                 MTexture(SharedData::instance().getRenderer(), c == ChessColor::WHITE ? "../resources/w_bishop.png"
                                                                                       : "../resources/b_bishop.png"), square) {
}

std::vector<std::pair<Direction, FDirection>> Bishop::getDirections() {
    return {{Direction::UP_LEFT,    &FDirector::up_left},
            {Direction::UP_RIGHT,   &FDirector::up_right},
            {Direction::DOWN_LEFT,  &FDirector::down_left},
            {Direction::DOWN_RIGHT, &FDirector::down_right}};
}

std::string Bishop::move_log(Square *, bool captured) {
    return HelperFunctions::get_algebraic_notation('B', m_square->getCoordinate(), captured);
}