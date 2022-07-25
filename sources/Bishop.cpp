#include "../headers/Bishop.h"
#include "../headers/HelperFunctions.h"

std::pair<std::vector<Square *>, std::vector<DirectionalSquares>> Bishop::moveable_squares(Square *) {
    FDirector fDirector(m_color);
    return HelperFunctions::get_squares_in_fdirections(m_board, m_square, &fDirector,
                                                       {{Direction::UPLEFT,    &FDirector::upleft},
                                                        {Direction::UPRIGHT,   &FDirector::upright},
                                                        {Direction::DOWNLEFT,  &FDirector::downleft},
                                                        {Direction::DOWNRIGHT, &FDirector::downright}});
}

bool Bishop::fide35(std::vector<DirectionalSquares> &legal_squares, Square *target) {
    auto this_indices = m_board->convert_coordinate_to_indices(getSquare()->getCoordinate());
    auto target_indices = m_board->convert_coordinate_to_indices(target->getCoordinate());

    bool up = target_indices.first > this_indices.first;
    bool left = target_indices.second < this_indices.second;

    Direction d;

    if (up && left) {
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