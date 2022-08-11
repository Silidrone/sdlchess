#include "../headers/King.h"
#include "../headers/Rook.h"

std::vector<Square *> King::attacked_squares() {
    return m_board->get_squares_in_fdirections(
            this,
            {
                    {Direction::UP,         &FDirector::up},
                    {Direction::DOWN,       &FDirector::down},
                    {Direction::LEFT,       &FDirector::left},
                    {Direction::RIGHT,      &FDirector::right},
                    {Direction::UP_LEFT,    &FDirector::up_left},
                    {Direction::UP_RIGHT,   &FDirector::up_right},
                    {Direction::DOWN_LEFT,  &FDirector::down_left},
                    {Direction::DOWN_RIGHT, &FDirector::down_right},
            }, true).first;
}

bool King::can_move_to_attacked(Square *s) {
    Piece *p = s->getPiece();
    return (p == nullptr || p->getColor() != getColor()) &&
           !s->isAttacked(static_cast<ChessColor>(!static_cast<bool>(getColor())));
}

std::vector<Square *> King::moveable_squares(std::vector<Square *> &attacked_squares) {
    std::vector<Square *> result;
    ChessColor c = getColor();
    auto oppositeColor = static_cast<ChessColor>(!static_cast<bool>(c));

    if (!hasMoved() && !getSquare()->isAttacked(oppositeColor)) {
        std::string kingSideRookCoordinate, queenSideRookCoordinate;
        if (c == ChessColor::WHITE) {
            kingSideRookCoordinate = "a8";
            queenSideRookCoordinate = "a1";
        } else {
            kingSideRookCoordinate = "h8";
            queenSideRookCoordinate = "h1";
        }

        Rook *kingSideRook = dynamic_cast<Rook *>(m_board->get_square_by_coordinate(
                kingSideRookCoordinate)->getPiece());
        Rook *queenSideRook = dynamic_cast<Rook *>(m_board->get_square_by_coordinate(
                queenSideRookCoordinate)->getPiece());
        if (kingSideRook != nullptr && !kingSideRook->hasMoved()) {
            Square *nextSquare = m_board->get_square_by_coordinate(m_fDirector.right(getSquare()->getCoordinate()));
            if (nextSquare->getPiece() == nullptr && !nextSquare->isAttacked(oppositeColor)) {
                result.push_back(m_board->get_square_by_coordinate(m_fDirector.right(nextSquare->getCoordinate())));
            }
        }
        if (queenSideRook != nullptr && !queenSideRook->hasMoved()) {
            Square *nextSquare = m_board->get_square_by_coordinate(m_fDirector.left(getSquare()->getCoordinate()));
            Square *nextNextSquare = m_board->get_square_by_coordinate(
                    m_fDirector.left(nextSquare->getCoordinate()));
            Square *nextNextNextSquare = m_board->get_square_by_coordinate(
                    m_fDirector.left(nextNextSquare->getCoordinate()));
            if (nextSquare->getPiece() == nullptr && nextNextSquare->getPiece() == nullptr &&
                nextNextNextSquare->getPiece() == nullptr && !nextSquare->isAttacked(oppositeColor) &&
                !nextNextSquare->isAttacked(oppositeColor)) {
                result.push_back(nextNextSquare);
            }
        }
    }

    auto squares = Piece::moveable_squares(attacked_squares);
    result.insert(result.end(), squares.begin(), squares.end());

    return result;
}

void King::post_move_f(Square *prev_square) {
    std::string current_coordinate = getSquare()->getCoordinate();
    std::string prev_coordinate = prev_square->getCoordinate();
    int current_column = current_coordinate[0] - 'a';
    int prev_column = prev_coordinate[0] - 'a';
    int col_diff = current_column - prev_column;
    auto current_row = current_coordinate[1];
    //if the king moved in the same row by two squares (i.e. castled)
    if (current_row == prev_coordinate[1] && abs(col_diff) == 2) {
        Rook *r = dynamic_cast<Rook *>(m_board->get_square_by_coordinate(
                std::string() + (col_diff > 0 ? 'h' : 'a') + current_row)->getPiece());
        if (r) {
            r->move_without_rules(m_board->get_square_by_coordinate(
                    col_diff > 0 ? m_fDirector.left(current_coordinate) : m_fDirector.right(current_coordinate)));
        }
    }
}