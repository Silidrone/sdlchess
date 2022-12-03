#ifndef CHESS_KING_H
#define CHESS_KING_H

#include "Piece.h"
#include "Square.h"

class King : public Piece {
public:
    King(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                                c == ChessColor::WHITE
                                                                                ? "../resources/w_king.png"
                                                                                : "../resources/b_king.png")) {}

    std::vector<Square *> attacked_squares() override;

    bool can_move_to_attacked(Square *) override;

    std::vector<Square *> moveable_squares(std::vector<Square *> &) override;

    void post_move_f(Square *) override;

    std::string move_log(Square *, bool) override;
};

#endif //CHESS_KING_H
