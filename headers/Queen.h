#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H

#include "Piece.h"

class Queen : public Piece {
public:
    Queen(ChessColor c, Board *b) : Piece(c, b, MTexture(SharedData::instance().getRenderer(),
                                                        c == ChessColor::WHITE ? "w_queen.png" : "b_queen.png")) {

    }

    bool move() override {
        
    }
};

#endif //CHESS_QUEEN_H
