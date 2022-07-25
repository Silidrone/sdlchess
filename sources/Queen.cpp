#include "../headers/Queen.h"
#include "../headers/Board.h"
#include "../headers/HelperFunctions.h"

std::pair<std::vector<Square *>, std::vector<DirectionalSquares>> Queen::moveable_squares(Square *) {
    FDirector fDirector(m_color);
    return HelperFunctions::get_squares_in_fdirections(m_board, m_square, &fDirector,
                                                       {{Direction::UP,        &FDirector::up},
                                                        {Direction::DOWN,      &FDirector::down},
                                                        {Direction::LEFT,      &FDirector::left},
                                                        {Direction::RIGHT,     &FDirector::right},
                                                        {Direction::UPLEFT,    &FDirector::upleft},
                                                        {Direction::UPRIGHT,   &FDirector::upright},
                                                        {Direction::DOWNLEFT,  &FDirector::downleft},
                                                        {Direction::DOWNRIGHT, &FDirector::downright}});
}

bool Queen::fide35(std::vector<DirectionalSquares> &legal_squares, Square *target) {
    auto this_indices = m_board->convert_coordinate_to_indices(getSquare()->getCoordinate());
    auto target_indices = m_board->convert_coordinate_to_indices(target->getCoordinate());

    bool up = target_indices.first > this_indices.first;
    bool same_row = target_indices.first == this_indices.first;
    bool left = target_indices.second < this_indices.second;
    bool same_column = target_indices.second == this_indices.second;
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
        d = Direction::UPLEFT;
    } else if (up) {
        d = Direction::UPRIGHT;
    } else if (left) {
        d = Direction::DOWNLEFT;
    } else {
        d = Direction::DOWNRIGHT;
    }

    return HelperFunctions::squaresHaveAnyPieces(HelperFunctions::get_squares_by_direction(legal_squares, d));
}