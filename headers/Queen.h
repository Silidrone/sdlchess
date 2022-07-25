#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H

#include "Piece.h"

class Queen : public Piece {
public:
    Queen(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                  c == ChessColor::WHITE ? "../resources/w_queen.png"
                                                                                         : "../resources/b_queen.png")) {

    }

    std::pair< std::vector<Square *>, std::vector<DirectionalSquares>> moveable_squares(Square *) override;
    bool fide35(std::vector<DirectionalSquares> &, Square *) override;
};

#endif //CHESS_QUEEN_H
