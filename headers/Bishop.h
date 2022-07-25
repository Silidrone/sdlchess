#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                   c == ChessColor::WHITE ? "../resources/w_bishop.png"
                                                                                          : "../resources/b_bishop.png")) {

    }

    std::pair< std::vector<Square *>, std::vector<DirectionalSquares>> moveable_squares(Square *) override;
    bool fide35(std::vector<DirectionalSquares> &, Square *) override;
};

#endif //CHESS_BISHOP_H
