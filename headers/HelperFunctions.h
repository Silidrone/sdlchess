#ifndef CHESS_HELPERFUNCTIONS_H
#define CHESS_HELPERFUNCTIONS_H

#include <string>
#include "Piece.h"

namespace HelperFunctions {
    std::string get_algebraic_notation(char, const std::string&, bool);
    Piece *getChosenPromotedPieceWithModal(ChessColor, Square*, Board*);
};

#endif //CHESS_HELPERFUNCTIONS_H
