#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(ChessColor, Square *, Board *);

    std::vector<Square *> attacked_squares() override;

    bool can_move_to_attacked(Square *) override;

    std::vector<Square *> moveable_squares(std::vector<Square *> &) override;
};

#endif //CHESS_PAWN_H
