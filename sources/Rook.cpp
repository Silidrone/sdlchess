#include "../headers/Rook.h"
#include "../headers/Square.h"
#include "../headers/HelperFunctions.h"

Rook::Rook(ChessColor c, Board *b, Square *square) : QRBPiece(c, b,
                                                              MTexture(SharedData::instance().getRenderer(),
                                                                       c == ChessColor::WHITE
                                                                       ? "../resources/w_rook.png"
                                                                       : "../resources/b_rook.png"), square) {}

std::vector<std::pair<Direction, FDirection>> Rook::getDirections() {
    return {
            {Direction::UP,    &FDirector::up},
            {Direction::DOWN,  &FDirector::down},
            {Direction::LEFT,  &FDirector::left},
            {Direction::RIGHT, &FDirector::right},
    };
}

std::string Rook::move_log(Square *, bool captured) {
    return HelperFunctions::get_algebraic_notation('R', m_square->getCoordinate(), captured);
}