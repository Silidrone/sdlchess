#include "../headers/Piece.h"
#include "../headers/Square.h"
#include "../headers/HelperFunctions.h"
#include "../headers/King.h"
#include <algorithm>

Piece::Piece(ChessColor c, Square *square, Board *board, const MTexture &texture)
        : ChessColored(c), m_square(nullptr), m_board(board), m_texture(texture), m_fDirector(c) {
    setSquare(square);
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

bool Piece::fide3p(const std::vector<Square *> &squares, Square *target) {
    return find_if(squares.begin(), squares.end(),
                   [&target](Square *s) { return s->getCoordinate() == target->getCoordinate(); }) !=
           squares.end();
}

//TODO finish the implementation of fide39 rule
bool Piece::fide39() {
    return true;
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

bool Piece::move(Square *target) {
    if(!fide12(target) || !fide31(target)) return false;
    auto squares_attacked = attacked_squares();
    auto legal_squares = moveable_squares(squares_attacked);
    bool move_legal = fide3p(legal_squares, target);
    if (move_legal) {
        setSquare(target);
        m_board->updateAttackedSquares();
    }

    return move_legal;
}

void Piece::setSquare(Square *square) {
    if (m_square) {
        m_square->removePiece();
    }
    square->putPiece(this);
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

std::pair<int, int> Piece::getPosition() const {
    return std::pair<int, int>(m_destination.x, m_destination.y);
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

void Piece::removePieceFromBoard() {
    m_board->removePiece(this);
}