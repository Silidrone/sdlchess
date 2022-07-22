#include "../headers/Rook.h"
#include <vector>
#include "../headers/Board.h"
#include "../headers/FDirections.h"
#include "../headers/HelperFunctions.h"
#include <algorithm>

bool Rook::move(Square *target) {
    //FIDE 3.1
    if (target->getPiece()->getColor() == this->getColor()) return false;

    //FIDE 3.3
    std::vector<Square *> legal_squares;
    auto up_squares = m_board->get_squares_in_direction_f(target, FDirection::up);
    legal_squares.insert(legal_squares.begin(), up_squares.begin(), up_squares.end());
    auto down_squares = m_board->get_squares_in_direction_f(target, FDirection::down);
    legal_squares.insert(legal_squares.begin(), up_squares.begin(), up_squares.end());
    if (find_if(legal_squares.begin(), legal_squares.end(),
                [&target](Square *s) { return s->getCoordinate() == target->getCoordinate(); }) ==
        legal_squares.end()) {
        return false;
    }

    //FIDE 3.5
    if(HelperFunctions::sqauresHaveAnyPieces(legal_squares)) {
        return false;
    }

    //TODO FIDE 3.9 check

    return true;
}