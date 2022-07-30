#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H

#include "QRBPiece.h"

class Bishop : public QRBPiece {
public:
    Bishop(ChessColor c, Square *square, Board *b) :
            QRBPiece(c, square, b,
                     MTexture(SharedData::instance().getRenderer(), c == ChessColor::WHITE ? "../resources/w_bishop.png"
                                                                                           : "../resources/b_bishop.png")) {}

    std::vector<std::pair<Direction, FDirection>> getDirections() override;
};

#endif //CHESS_BISHOP_H
