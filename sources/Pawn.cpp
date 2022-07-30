#include "../headers/Pawn.h"
#include "../headers/Square.h"
#include <iostream>

std::vector<Square *> Pawn::moveable_squares(Square *target) {
    std::vector<Square *> result;
    const auto current_coordinate = m_square->getCoordinate();
    const auto next_up_square = m_board->get_square_by_coordinate(FDirector::up(current_coordinate));
    result.push_back(next_up_square);
    const auto row_coordinate = m_square->getCoordinate().at(1);
    const auto color = getColor();
    std::cout << "this square: " << getSquare()->getCoordinate() << std::endl;
    std::cout << "next square: " << next_up_square->getCoordinate() << std::endl;
    std::cout << "target square: " << target->getCoordinate() << std::endl;

    // if the pawn is in the starting position allow it to move to the square after the next one too (in the up direction)
    if ((row_coordinate == '2' && color == ChessColor::WHITE) ||
        (row_coordinate == '7' && color == ChessColor::BLACK)) {
        result.push_back(m_board->get_square_by_coordinate(FDirector::up(next_up_square->getCoordinate())));
    }
    // if the target square contains a piece and if it is to the upleft or to the upright of the pawn, add it to result
    if (target->getPiece() != nullptr || FDirector::up_left(current_coordinate) == target->getCoordinate() ||
        FDirector::up_right(current_coordinate) == target->getCoordinate()) {
        result.push_back(target);
    }


    return result;
}
