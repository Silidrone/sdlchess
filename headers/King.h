#ifndef CHESS_KING_H
#define CHESS_KING_H

#include "Piece.h"

class King : public Piece {
public:
    King(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                                c == ChessColor::WHITE ? "../resources/w_king.png"
                                                                                                       : "../resources/b_king.png")) {

    }

    std::pair< std::vector<Square *>, std::vector<DirectionalSquares>> moveable_squares(Square *) override;
    bool fide35(std::vector<DirectionalSquares> &, Square *) override;
};

#endif //CHESS_KING_H
