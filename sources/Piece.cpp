#include "../headers/Piece.h"
#include "../headers/Square.h"
#include "../headers/King.h"
#include <algorithm>
#include "../headers/Pawn.h"
#include "../headers/MoveLogger.h"
#include "../headers/HelperFunctions.h"
#include <functional>

Piece::Piece(ChessColor c, Board *board, const MTexture &texture, Square *square)
        : ChessColored(c), m_board(board), m_texture(texture), m_fDirector(c), m_moved(false) {
    if(square) {
        square->putPiece(this);
        setSquare(square);
    }
}

Piece::~Piece() {}

//From fide 1.2: "...and also ’capturing’ the opponent’s king are not allowed"
bool Piece::fide12(Square *target) {
    return dynamic_cast<King *>(target->getPiece()) == nullptr;
}

bool Piece::fide31(Square *target) {
    Piece *target_piece = target->getPiece();
    return target_piece == nullptr || (target_piece->getColor() != getColor());
}

bool Piece::fide39() {
    King *king = m_board->getKing(getColor());
    return king == nullptr || !king->getSquare()->isAttacked(static_cast<ChessColor>(!static_cast<bool>(getColor())));
}

std::vector<Square *> Piece::moveable_squares(std::vector<Square *> &attacked_squares) {
    std::vector<Square *> result;
    for (auto &attacked_square: attacked_squares) {
        if (can_move_to_attacked(attacked_square)) {
            result.push_back(attacked_square);
        }
    }

    return result;
}

void Piece::attack_squares() {
    m_squares_attacked = attacked_squares();
    for (auto &attacked_square: m_squares_attacked) {
        attacked_square->attack(m_color);
    }
}

void Piece::unattack_squares() {
    for (auto &attacked_square: m_squares_attacked) {
        attacked_square->unattack(m_color);
    }

    m_squares_attacked.clear();
}

bool Piece::move(Square *target, bool test_move, std::function<Piece*(Pawn *)> promotion_method) {
    if (!fide12(target) || !fide31(target)) return false;
    auto squares_attacked = attacked_squares();
    auto legal_squares = moveable_squares(squares_attacked);

    if (std::find_if(legal_squares.begin(), legal_squares.end(),
                     [&target](Square *s) { return s->getCoordinate() == target->getCoordinate(); }) ==
        legal_squares.end()) {
        return false;
    }

    Piece *target_piece = target->getPiece();
    auto rp = move_without_rules(target);
    Square *previous_square = rp.first;
    bool target_piece_removed = rp.second;
    bool bfide39 = fide39();

    Pawn *pawn = dynamic_cast<Pawn *>(this);
    bool promotion_cancelled = false;
    char target_square_row = target->getCoordinate()[1];
    if(bfide39 && pawn && promotion_method && (target_square_row == '8' || target_square_row == '1')) {
        Piece *chosen_promoted_piece = promotion_method(pawn);
        if(chosen_promoted_piece) {
            m_square->putPiece(chosen_promoted_piece);
            chosen_promoted_piece->setSquare(m_square);
            pawn->setPromotedPiece(chosen_promoted_piece);
        } else {
            promotion_cancelled = true;
        }
    }

    if (!test_move && bfide39 && !promotion_cancelled) {
        post_move_f(previous_square);
        m_board->getMoveLogger().addLog(this, move_log(previous_square, target_piece != nullptr),
                                        previous_square->getCoordinate(), m_square->getCoordinate());
        m_moved = true;
        return true;
    } else {
        this->setSquare(previous_square);
        previous_square->putPiece(this);
        if (target_piece) {
            target->putPiece(target_piece);
            if (target_piece_removed) m_board->unRemoveLastPiece();
        } else {
            target->removePiece();
        }
        m_board->updateAttackedSquares();
    }

    if (pawn) {
        pawn->setPromotedPiece(nullptr);
    }

    return bfide39 && !promotion_cancelled;
}

void Piece::post_move_f(Square *) {}

std::string Piece::move_log(Square *, bool) { return ""; }

std::pair<Square *, bool> Piece::move_without_rules(Square *target) {
    bool target_piece_removed = false;
    if (m_square) {
        m_square->removePiece();
    }
    Piece *target_piece = target->getPiece();
    if (target_piece) {
        target_piece->unattack_squares();
        target_piece->removeFromBoard();
        target->removePiece();
        target_piece_removed = true;
    }
    target->putPiece(this);
    Square *previous_square = m_square;
    this->setSquare(target);

    m_board->updateAttackedSquares();
    return {previous_square, target_piece_removed};
}

void Piece::setSquare(Square *square) {
    m_square = square;
    m_destination = square->getDestination();
}

void Piece::resetPosition() {
    m_destination = m_square->getDestination();
}

void Piece::setPosition(int x, int y) {
    m_destination.x = x - m_destination.w / 2;
    m_destination.y = y - m_destination.h / 2;
}

void Piece::render() {
    m_texture.render(m_destination);
}

Square *Piece::getSquare() const { return m_square; }

void Piece::setRenderPriority(int rp) {
    m_render_priority = rp;
    m_board->updateRenderOrder();
}

int Piece::getRenderPriority() const {
    return m_render_priority;
}

FDirector Piece::getFDirector() const {
    return m_fDirector;
}

void Piece::removeFromBoard() {
    m_board->removePiece(this);
}

bool Piece::hasMoved() const {
    return m_moved;
}