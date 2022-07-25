#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                   c == ChessColor::WHITE ? "../resources/w_knight.png"
                                                                                          : "../resources/b_knight.png")) {

    }

    std::pair< std::vector<Square *>, std::vector<DirectionalSquares>> moveable_squares(Square *) override;
    bool fide35(std::vector<DirectionalSquares> &, Square *) override;
};

#endif //CHESS_KNIGHT_H
