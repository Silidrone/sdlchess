#ifndef CHESS_HELPERFUNCTIONS_H
#define CHESS_HELPERFUNCTIONS_H

#include <string>
#include "Piece.h"
#include "../headers/PGNGameDetails.h"

namespace HelperFunctions {
    std::string get_algebraic_notation(char, const std::string &, bool);

    Piece *getChosenPromotedPieceWithModal(ChessColor, SDL_Rect, Board *);

    std::vector<std::string> split(const std::string &, char);

    std::string without(std::string, char);

    std::string &rtrim(std::string &s);

    std::string &ltrim(std::string &s);

    std::string &trim(std::string &s);

    Piece *matchingPiece(const std::string &target, std::vector<Piece *> &possible_pieces,
                         char column = '\0', char row = '\0');

    std::vector<PGNGameDetails> parsePGN(std::string &&);

    ChessColor oppositeColor(ChessColor c);
};

#endif //CHESS_HELPERFUNCTIONS_H
