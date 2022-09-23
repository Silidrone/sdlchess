#include "../headers/Pawn.h"
#include "../headers/Square.h"
#include "../headers/MoveLogger.h"
#include <iostream>
#include "../headers/Queen.h"

Pawn::Pawn(ChessColor c, Square *square, Board *b) : Piece(c, square, b, MTexture(SharedData::instance().getRenderer(),
                                                                                  c == ChessColor::WHITE
                                                                                  ? "../resources/w_pawn.png"
                                                                                  : "../resources/b_pawn.png")),
                                                     m_en_passed_square(nullptr) {
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
    if (next_up_square && next_up_square->getPiece() == nullptr) result.push_back(next_up_square);

    // if the pawn hasn't moved yet, allow it to move to the square after the next one too (in the up direction)
    if (!hasMoved()) {
        auto s = m_board->get_square_by_coordinate(m_fDirector.up(next_up_square->getCoordinate()));
        if (s && s->getPiece() == nullptr) {
            result.push_back(s);
        }
    }

    Square *neighbour_squares[2] = {
            m_board->get_square_by_coordinate(m_fDirector.left(current_coordinate)),
            m_board->get_square_by_coordinate(m_fDirector.right(current_coordinate))
    };
    for (auto &neighbour_square: neighbour_squares) {
        if (neighbour_square) {
            Pawn *neighbour_pawn = dynamic_cast<Pawn *>(neighbour_square->getPiece());
            if (neighbour_pawn && neighbour_pawn->getColor() != m_color) {
                auto logs = MoveLogger::instance().getLogs(neighbour_pawn);
                if (logs.size() > 0 && MoveLogger::instance().getCurrentMoveCount() - logs.back().move_count == 1 &&
                    abs(logs.back().prev[1] - logs.back().current[1]) == 2) {
                    result.push_back(m_board->get_square_by_coordinate(m_fDirector.up(logs.back().current)));
                    m_en_passed_square = neighbour_square;
                }
            }
        }
    }

    auto squares = Piece::moveable_squares(attacked_squares);
    result.insert(result.end(), squares.begin(), squares.end());

    return result;
}

void Pawn::post_move_f(Square *) {
    if (m_en_passed_square) {
        auto en_passed_pawn = m_en_passed_square->getPiece();
        en_passed_pawn->unattack_squares();
        en_passed_pawn->removeFromBoard();
        m_en_passed_square->removePiece();
        m_en_passed_square = nullptr;
    }
    Square *current_square = getSquare();
    int current_row = current_square->getCoordinate()[1] - '0';
    if(current_row == 1 || current_row == 8) {
        this->unattack_squares();
        m_board->removePiece(this);
        current_square->removePiece();
        Queen *promoted = new Queen(getColor(), current_square, m_board);
        m_board->addPiece(promoted);
        m_board->updateAttackedSquares();
        delete this;
    }
}