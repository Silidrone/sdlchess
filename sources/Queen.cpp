#include "../headers/Queen.h"
#include "../headers/Board.h"
#include "../headers/HelperFunctions.h"

std::pair<std::vector<Square *>, std::vector<DirectionalSquares>> Queen::moveable_squares(Square *) {
    return HelperFunctions::get_squares_in_fdirections(m_board, m_square,
                                                       {{Direction::UP,         FDirector::up},
                                                        {Direction::DOWN,       FDirector::down},
                                                        {Direction::LEFT,       FDirector::left},
                                                        {Direction::RIGHT,      FDirector::right},
                                                        {Direction::UP_LEFT,    FDirector::up_left},
                                                        {Direction::UP_RIGHT,   FDirector::up_right},
                                                        {Direction::DOWN_LEFT,  FDirector::down_left},
                                                        {Direction::DOWN_RIGHT, FDirector::down_right}});
}

bool Queen::fide35(std::vector<DirectionalSquares> &legal_squares, Square *target) {
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
    } else if (up && left) {
        d = Direction::UP_LEFT;
    } else if (up) {
        d = Direction::UP_RIGHT;
    } else if (left) {
        d = Direction::DOWN_LEFT;
    } else {
        d = Direction::DOWN_RIGHT;
    }

    return HelperFunctions::squaresBeforeNextPiece(
            HelperFunctions::get_squares_by_direction(legal_squares, d)).size() > 0;
}