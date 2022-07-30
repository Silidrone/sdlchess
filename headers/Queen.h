#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H

#include "QRBPiece.h"

class Queen : public QRBPiece {
public:
    Queen(ChessColor c, Square *square, Board *b) :
            QRBPiece(c, square, b, MTexture(SharedData::instance().getRenderer(), c == ChessColor::WHITE
                                                                                  ? "../resources/w_queen.png"
                                                                                  : "../resources/b_queen.png")) {

    }

    std::vector<std::pair<Direction, FDirection>> getDirections() override;
};

#endif //CHESS_QUEEN_H
