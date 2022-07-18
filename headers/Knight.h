#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                   c == ChessColor::WHITE ? "../resources/w_knight.png"
                                                                                          : "../resources/b_knight.png")) {

    }

    void move() override {
        
    }
};

#endif //CHESS_KNIGHT_H
