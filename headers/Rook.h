#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H

#include "Piece.h"

class Rook : public Piece {
public:
    Rook(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                 c == ChessColor::WHITE ? "../resources/w_rook.png"
                                                                                        : "../resources/b_rook.png")) {

    }

    std::pair< std::vector<Square *>, std::vector<DirectionalSquares>> moveable_squares(Square *) override;
    bool fide35(std::vector<DirectionalSquares> &, Square *) override;
};

#endif //CHESS_ROOK_H
