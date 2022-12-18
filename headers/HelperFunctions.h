#ifndef CHESS_HELPERFUNCTIONS_H
#define CHESS_HELPERFUNCTIONS_H

#include <string>
#include "Piece.h"
#include "../headers/PGNGameDetails.h"

namespace HelperFunctions {
    std::string get_algebraic_notation(char, const std::string&, bool);
    Piece *getChosenPromotedPieceWithModal(ChessColor, Square*, Board*);
    std::vector<std::string> split (const std::string &, char);
    std::string& rtrim(std::string& s);
    std::string& ltrim(std::string& s);
    std::string& trim(std::string& s);

    std::vector<PGNGameDetails> parsePGN(std::string &&);
};

#endif //CHESS_HELPERFUNCTIONS_H
