#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                 c == ChessColor::WHITE ? "../resources/w_pawn.png"
                                                                                        : "../resources/b_pawn.png")) {

    }

    bool move(Square *target) override;
};

#endif //CHESS_PAWN_H
