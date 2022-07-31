#ifndef CHESS_QRBPIECE_H
#define CHESS_QRBPIECE_H

#include "Piece.h"

class QRBPiece : public Piece {
private:
    std::vector<Square *> squaresBeforeNextPieceInDirection(const DirectionalSquares &);
    std::vector<Square *> squaresBeforeNextPieceInDirections(const std::vector<DirectionalSquares>&);
public:
    QRBPiece(ChessColor, Square *, Board *, const MTexture &);
    virtual std::vector<std::pair<Direction, FDirection>> getDirections() = 0;
    std::vector<Square *> moveable_squares(Square *) override;

};

#endif //CHESS_QRBPIECE_H