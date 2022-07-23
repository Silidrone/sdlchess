#include "../headers/Pawn.h"
#include "../headers/Square.h"
#include "../headers/Board.h"

std::vector<Square *> Pawn::moveable_squares(Square *target) {
    std::vector<Square *> result;
    const auto next_up_square = m_board->get_square_by_indices_pair(
            FDirections::up(m_board->convert_coordinate_to_indices(m_square->getCoordinate())));
    result.push_back(next_up_square);
    const auto row_coordinate = m_square->getCoordinate().at(1);
    const auto color = getColor();
    // if the pawn is in the starting position allow it to move to the square after the next one too (in the up direction)
    if ((row_coordinate == '2' && color == ChessColor::WHITE) ||
        (row_coordinate == '7' && color == ChessColor::BLACK)) {
        result.push_back(m_board->get_square_by_indices_pair(
                FDirections::up(m_board->convert_coordinate_to_indices(next_up_square->getCoordinate()))));
    }
    if (target->getPiece() != nullptr) {
        result.push_back(target);
    }

    return result;
}