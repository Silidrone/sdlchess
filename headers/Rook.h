#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H

#include "Piece.h"

class Rook : public Piece {
public:
    Rook(ChessColor c, Board *b) : Piece(c, b, MTexture(SharedData::instance().getRenderer(),
                                                        c == ChessColor::WHITE ? "w_rook.png" : "b_rook.png")) {

    }

    bool move() override {
        
    }
};

#endif //CHESS_ROOK_H
