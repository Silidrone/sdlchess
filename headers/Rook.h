#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H

#include "QRBPiece.h"

class Rook : public QRBPiece {
public:
    Rook(ChessColor c, Square *square, Board *b) : QRBPiece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                 c == ChessColor::WHITE ? "../resources/w_rook.png"
                                                                                        : "../resources/b_rook.png")) {

    }

    std::vector<std::pair<Direction, FDirection>> getDirections() override;
};

#endif //CHESS_ROOK_H
