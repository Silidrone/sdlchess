#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(ChessColor, Board *, Square * = nullptr);

    std::vector<Square *> attacked_squares() override;

    bool can_move_to_attacked(Square *) override;
    std::string move_log(Square *, bool) override;
};

#endif //CHESS_KNIGHT_H
