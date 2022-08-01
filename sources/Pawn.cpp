#include "../headers/Pawn.h"
#include "../headers/Square.h"

Pawn::Pawn(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                                  c == ChessColor::WHITE
                                                                                  ? "../resources/w_pawn.png"
                                                                                  : "../resources/b_pawn.png")) {
}

std::vector<Square *> Pawn::attacked_squares() {
    const auto current_coordinate = m_square->getCoordinate();
    std::vector<Square *> result;
    auto sul = m_board->get_square_by_coordinate(m_fDirector.up_left(current_coordinate));
    auto sur = m_board->get_square_by_coordinate(m_fDirector.up_right(current_coordinate));
    if (sul) result.push_back(sul);
    if (sur) result.push_back(sur);
    return result;
}

bool Pawn::can_move_to_attacked(Square *target) {
    return target->getPiece() != nullptr;
}

std::vector<Square *> Pawn::moveable_squares(std::vector<Square *> &attacked_squares) {
    std::vector<Square *> result;
    const auto current_coordinate = m_square->getCoordinate();
    const auto next_up_square = m_board->get_square_by_coordinate(m_fDirector.up(current_coordinate));
    if (next_up_square) result.push_back(next_up_square);
    const auto row_coordinate = m_square->getCoordinate().at(1);
    const auto color = getColor();

    // if the pawn is in the starting position allow it to move to the square after the next one too (in the up direction)
    if ((row_coordinate == '2' && color == ChessColor::WHITE) ||
        (row_coordinate == '7' && color == ChessColor::BLACK)) {
        auto s = m_board->get_square_by_coordinate(m_fDirector.up(next_up_square->getCoordinate()));
        if (s) result.push_back(s);
    }

    auto squares = Piece::moveable_squares(attacked_squares);
    result.insert(result.end(), squares.begin(), squares.end());

    return result;
}