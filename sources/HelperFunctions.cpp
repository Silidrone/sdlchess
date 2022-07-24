#include "../headers/HelperFunctions.h"

bool HelperFunctions::squaresHaveAnyPieces(std::vector<Square *> squares) {
    for (auto &square: squares) {
        if (square->getPiece() != nullptr) return true;
    }

    return false;
}

std::vector<Square *>
HelperFunctions::get_squares_in_fdirections(Board *board, Square *beginning_square, FDirector *fDirector, std::vector<FDirection> directions,
                                            bool one_step) {
    std::vector<Square *> result_squares;
    for (auto &direction: directions) {
        auto squares = board->get_squares_in_fdirection(beginning_square, fDirector, direction, one_step);
        result_squares.insert(result_squares.begin(), squares.begin(), squares.end());
    }

    return result_squares;
}
