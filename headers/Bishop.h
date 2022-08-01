#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H

#include "QRBPiece.h"

class Bishop : public QRBPiece {
public:
    Bishop(ChessColor, Square *, Board *);

    std::vector<std::pair<Direction, FDirection>> getDirections() override;
};

#endif //CHESS_BISHOP_H
