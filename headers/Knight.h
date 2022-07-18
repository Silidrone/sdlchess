#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(ChessColor c, Board *b) : Piece(c, b, MTexture(SharedData::instance().getRenderer(),
                                                        c == ChessColor::WHITE ? "w_knight.png" : "b_knight.png")) {

    }

    bool move() override {
        
    }
};

#endif //CHESS_KNIGHT_H
