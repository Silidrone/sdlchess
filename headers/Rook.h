#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H

#include "Piece.h"

class Rook : public Piece {
public:
    Rook(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                 c == ChessColor::WHITE ? "../resources/w_rook.png"
                                                                                        : "../resources/b_rook.png")) {

    }

    void move() override {
        
    }
};

#endif //CHESS_ROOK_H
