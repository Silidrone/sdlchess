#ifndef CHESS_KING_H
#define CHESS_KING_H

#include "Piece.h"

class King : public Piece {
public:
    King(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                                c == ChessColor::WHITE ? "../resources/w_king.png"
                                                                                                       : "../resources/b_king.png")) {

    }

    void move() override {

    }
};

#endif //CHESS_KING_H
