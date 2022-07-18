#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(ChessColor c, Board *b) : Piece(c, b, MTexture(SharedData::instance().getRenderer(),
                                                        c == ChessColor::WHITE ? "w_pawn.png" : "b_pawn.png")) {

    }

    bool move() override {
        
    }
};

#endif //CHESS_PAWN_H
