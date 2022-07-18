#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(ChessColor c, Board *b) : Piece(c, b, MTexture(SharedData::instance().getRenderer(),
                                                        c == ChessColor::WHITE ? "w_bishop.png" : "b_bishop.png")) {

    }

    bool move() override {
        
    }
};

#endif //CHESS_BISHOP_H
