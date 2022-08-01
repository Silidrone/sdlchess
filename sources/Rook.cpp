#include "../headers/Rook.h"

Rook::Rook(ChessColor c, Square *square, Board *b) : QRBPiece(c, square, b,
                                                              MTexture(SharedData::instance().getRenderer(),
                                                                       c == ChessColor::WHITE
                                                                       ? "../resources/w_rook.png"
                                                                       : "../resources/b_rook.png")) {}

std::vector<std::pair<Direction, FDirection>> Rook::getDirections() {
    return {
            {Direction::UP,    &FDirector::up},
            {Direction::DOWN,  &FDirector::down},
            {Direction::LEFT,  &FDirector::left},
            {Direction::RIGHT, &FDirector::right},
    };
}