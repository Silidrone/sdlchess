#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                   c == ChessColor::WHITE ? "../resources/w_bishop.png"
                                                                                          : "../resources/b_bishop.png")) {

    }

    void move() override {
        
    }
};

#endif //CHESS_BISHOP_H
