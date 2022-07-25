#include "../headers/Pawn.h"
#include "../headers/Square.h"
#include "../headers/Board.h"

std::pair< std::vector<Square *>, std::vector<DirectionalSquares>>Pawn::moveable_squares(Square *target) {
    FDirector fDirector(m_color);
    std::vector<Square *> result;
    const auto current_indices = m_board->convert_coordinate_to_indices(m_square->getCoordinate());
    const auto next_up_square = m_board->get_square_by_indices_pair(fDirector.up(current_indices));
    result.push_back(next_up_square);
    const auto row_coordinate = m_square->getCoordinate().at(1);
    const auto color = getColor();

    // if the pawn is in the starting position allow it to move to the square after the next one too (in the up direction)
    if ((row_coordinate == '2' && color == ChessColor::WHITE) ||
        (row_coordinate == '7' && color == ChessColor::BLACK)) {
        const auto next_up_square_indices = m_board->convert_coordinate_to_indices(next_up_square->getCoordinate());
        result.push_back(m_board->get_square_by_indices_pair(fDirector.up(next_up_square_indices)));
    }
    if (target->getPiece() != nullptr &&
        (m_board->get_square_by_indices_pair(fDirector.upleft(current_indices))->getCoordinate() ==
         target->getCoordinate() ||
         m_board->get_square_by_indices_pair(fDirector.upright(current_indices))->getCoordinate() ==
         target->getCoordinate())) {
        result.push_back(target);
    }

    return {result, {}};
}

bool Pawn::fide35(std::vector<DirectionalSquares> &, Square *) { return true; }