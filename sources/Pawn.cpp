#include "../headers/Pawn.h"
#include "../headers/Square.h"
#include "../headers/MoveLogger.h"
#include "../headers/Queen.h"
#include "../headers/HelperFunctions.h"
#include "../headers/Rook.h"
#include "../headers/Bishop.h"
#include "../headers/Knight.h"

Pawn::Pawn(ChessColor c, Board *b, Square *square) : Piece(c, b, MTexture(SharedData::instance().getRenderer(),
                                                                                  c == ChessColor::WHITE
                                                                                  ? "../resources/w_pawn.png"
                                                                                  : "../resources/b_pawn.png"), square),
                                                     m_en_passed_square(nullptr), m_promoted_piece(nullptr) {
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

void Pawn::setEnPassedSquare(Square *s) {
    m_en_passed_square = s;
}

std::vector<Square *> Pawn::moveable_squares(std::vector<Square *> &attacked_squares) {
    std::vector<Square *> result;
    const auto current_coordinate = m_square->getCoordinate();
    const auto next_up_square = m_board->get_square_by_coordinate(m_fDirector.up(current_coordinate));
    if (next_up_square && next_up_square->getPiece() == nullptr) {
        result.push_back(next_up_square);
        // if the pawn hasn't moved yet, allow it to move to the square after the next one too (in the up direction)
        if (!hasMoved()) {
            auto s = m_board->get_square_by_coordinate(m_fDirector.up(next_up_square->getCoordinate()));
            if (s && s->getPiece() == nullptr) {
                result.push_back(s);
            }
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
                auto logs = m_board->getMoveLogger().getLogs(neighbour_pawn);
                if (logs.size() > 0 && m_board->getMoveLogger().getCurrentMoveCount() - logs.back().move_count == 1 &&
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

std::function<void()> Pawn::move_without_rules(Square *target) {
    Piece *removed_piece = nullptr;
    Square *previous_square = m_square;
    Square *en_passed_square = m_en_passed_square;
    if (en_passed_square && en_passed_square->getCoordinate()[0] == target->getCoordinate()[0]) {
        auto en_passed_pawn = en_passed_square->getPiece();
        removed_piece = en_passed_pawn;
        en_passed_pawn->unattack_squares();
        m_en_passed_square->removePiece();
        en_passed_pawn->removeFromBoard();

        m_square->removePiece();
        target->putPiece(this);
        this->setSquare(target);
        m_board->updateAttackedSquares();

        return [=]() {
            this->setSquare(previous_square);
            previous_square->putPiece(this);
            if (removed_piece) {
                en_passed_square->putPiece(removed_piece);
                m_board->unRemoveLastPiece();
            }
            target->removePiece();
            m_board->updateAttackedSquares();
            m_en_passed_square = en_passed_square;
        };
    } else {
        return Piece::move_without_rules(target);
    }
}

void Pawn::post_move_f(Square *previous_square) {
    if (m_promoted_piece) {
        this->unattack_squares();
        this->removeFromBoard();
        m_board->addPiece(m_promoted_piece);
        m_board->updateAttackedSquares();
        m_promoted_piece = nullptr;
    }
}

std::string Pawn::move_log(Square *prev, bool captured) {
    std::string current_coordinate = m_square->getCoordinate();
    std::string prev_coordinate = prev->getCoordinate();
    std::string result = "";
    //if en passant
    if (!captured && prev_coordinate[0] != current_coordinate[0] && prev_coordinate[1] != current_coordinate[1]) {
        captured = true;
    }
    if (captured) {
        result += std::string() + prev->getCoordinate()[0] + 'x' + current_coordinate;
    } else {
        result += current_coordinate;
    }
    if (current_coordinate[1] == '1' || current_coordinate[1] == '8') {
        char c;
        if (dynamic_cast<Queen *>(m_promoted_piece)) {
            c = 'Q';
        } else if (dynamic_cast<Rook *>(m_promoted_piece)) {
            c = 'R';
        } else if (dynamic_cast<Bishop *>(m_promoted_piece)) {
            c = 'B';
        } else if (dynamic_cast<Knight *>(m_promoted_piece)) {
            c = 'N';
        }
        result += c;
    }

    return result;
}

void Pawn::setPromotedPiece(Piece *p) {
    m_promoted_piece = p;
}