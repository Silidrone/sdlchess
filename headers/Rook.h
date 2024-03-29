#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H

#include "QRBPiece.h"

class Rook : public QRBPiece {
public:
    Rook(ChessColor, Board *, Square * = nullptr);

    std::vector<std::pair<Direction, FDirection>> getDirections() override;
    std::string move_log(Square *, bool) override;
};

#endif //CHESS_ROOK_H
