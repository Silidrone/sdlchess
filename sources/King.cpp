#include "../headers/King.h"
#include "../headers/Square.h"
#include "../headers/Board.h"

std::pair<std::vector<Square *>, std::vector<DirectionalSquares>> King::moveable_squares(Square *target) {
    // TODO finish after adding "attacked" property to a square as King has to be able to know if the squares he can move to are attacked or not
    return {};
}

bool King::fide35(std::vector<DirectionalSquares> &, Square *) { return true; }