#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H

#include "QRBPiece.h"

class Rook : public QRBPiece {
public:
    Rook(ChessColor, Square *, Board *);

    std::vector<std::pair<Direction, FDirection>> getDirections() override;
};

#endif //CHESS_ROOK_H
