#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H

#include "QRBPiece.h"

class Queen : public QRBPiece {
public:
    Queen(ChessColor, Square *, Board *);

    std::vector<std::pair<Direction, FDirection>> getDirections() override;
};

#endif //CHESS_QUEEN_H
