#include "../headers/HelperFunctions.h"

bool HelperFunctions::squaresHaveAnyPieces(std::vector<Square *> squares) {
    for (auto &square: squares) {
        if (square->getPiece() != nullptr) return true;
    }

    return false;
}

std::pair<std::vector<Square *>, std::vector<DirectionalSquares>>
HelperFunctions::get_squares_in_fdirections(Board *board, Square *beginning_square, FDirector *fDirector,
                                            std::vector<std::pair<Direction, FDirection>> directions,
                                            bool one_step) {
    std::vector<DirectionalSquares> result;
    std::vector<Square *> all_squares;
    for (auto &direction: directions) {
        auto squares = board->get_squares_in_fdirection(beginning_square, fDirector, direction.second, one_step);
        result.push_back({direction.first, squares});
        all_squares.insert(all_squares.begin(), squares.begin(), squares.end());
    }

    return {all_squares, result};
}

std::vector<Square *>
HelperFunctions::get_squares_by_direction(std::vector<DirectionalSquares> &legal_squares, Direction d) {
    for (auto &legal_square: legal_squares) {
        if (legal_square.first == d) {
            return legal_square.second;
        }
    }

    return std::vector<Square *>();
}