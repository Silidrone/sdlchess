#ifndef CHESS_KING_H
#define CHESS_KING_H

#include "Piece.h"
#include "Square.h"

class King : public Piece {
public:
    King(ChessColor c, Board *b, Square *square = nullptr) : Piece(c, b, MTexture(SharedData::instance().getRenderer(),
                                                                                c == ChessColor::WHITE
                                                                                ? (SharedData::instance().getResourcesPath() + "w_king.png").c_str()
                                                                                : (SharedData::instance().getResourcesPath() + "b_king.png").c_str()), square) {}

    std::vector<Square *> attacked_squares() override;

    bool can_move_to_attacked(Square *) override;

    std::vector<Square *> moveable_squares(std::vector<Square *> &) override;

    void post_move_f(Square *) override;

    std::string move_log(Square *, bool) override;
};

#endif //CHESS_KING_H
