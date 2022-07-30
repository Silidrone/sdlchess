#include "../headers/Rook.h"
#include <vector>
#include "../headers/Board.h"
#include <algorithm>
#include "../headers/HelperFunctions.h"

std::pair<std::vector<Square *>, std::vector<DirectionalSquares>> Rook::moveable_squares(Square *) {
    return HelperFunctions::get_squares_in_fdirections(m_board, m_square,
                                                       {{Direction::UP,   FDirector::up},
                                                        {Direction::DOWN, FDirector::down}});
}

bool Rook::fide35(std::vector<DirectionalSquares> &legal_squares, Square *target) {
    auto this_coordinate = getSquare()->getCoordinate();
    auto target_coordinate = target->getCoordinate();

    bool up = target_coordinate[1] > this_coordinate[1];
    bool same_row = target_coordinate[1] == this_coordinate[1];
    bool left = target_coordinate[0] < this_coordinate[0];
    bool same_column = target_coordinate[0] == this_coordinate[0];
    Direction d;

    if (same_column) {
        if (up) {
            d = Direction::UP;
        } else {
            d = Direction::DOWN;
        }
    } else if (same_row) {
        if (left) {
            d = Direction::LEFT;
        } else {
            d = Direction::RIGHT;
        }
    }

    return HelperFunctions::squaresBeforeNextPiece(HelperFunctions::get_squares_by_direction(legal_squares, d)).size() > 0;
}