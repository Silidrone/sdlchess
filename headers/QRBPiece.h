#ifndef CHESS_QRBPIECE_H
#define CHESS_QRBPIECE_H

#include "Piece.h"

class QRBPiece : public Piece {
private:
    std::vector<Square *> squaresBeforeNextPieceInDirection(const DirectionalSquares &);
    std::vector<Square *> squaresBeforeNextPieceInDirections(const std::vector<DirectionalSquares>&);
public:
    QRBPiece(ChessColor, Board *, const MTexture &, Square * = nullptr);
    virtual ~QRBPiece();
    virtual std::vector<std::pair<Direction, FDirection>> getDirections() = 0;
    std::vector<Square *> attacked_squares() override;
    bool can_move_to_attacked(Square *) override;
};

#endif //CHESS_QRBPIECE_H
