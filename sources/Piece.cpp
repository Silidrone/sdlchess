#include "../headers/Piece.h"
#include "../headers/Square.h"
#include "../headers/HelperFunctions.h"
#include <algorithm>

Piece::Piece(ChessColor c, Square *square, Board *board, const MTexture &texture)
        : ChessColored(c), m_square(nullptr), m_board(board), m_texture(texture) {
    setSquare(square);
}

//From fide 1.2: "...and also ’capturing’ the opponent’s king are not allowed"
bool Piece::fide12(Square *target) {
    return dynamic_cast<King *>(target->getPiece()) == nullptr;
}

bool Piece::fide31(Square *target) {
    Piece *target_piece = target->getPiece();
    return target_piece == nullptr || (target_piece->getColor() != this->getColor());
}

bool Piece::fide3p(std::vector<Square *> &legal_squares, Square *target) {
    return find_if(legal_squares.begin(), legal_squares.end(),
                   [&target](Square *s) { return s->getCoordinate() == target->getCoordinate(); }) !=
           legal_squares.end();
}

//TODO finish the implementation of fide39 rule
bool Piece::fide39() {
    return true;
}

bool Piece::move(Square *target) {
    auto legal_squares = moveable_squares(target);
    bool bfide12 = fide12(target);
    bool bfide31 = fide31(target);
    bool bfide3p = fide3p(legal_squares.first, target);
    bool bfide35 = fide35(legal_squares.second, target);
    bool bfide39 = fide39();
    bool move_legal = bfide12 && bfide31 && bfide3p && bfide35 && bfide39;
    if(move_legal) {
        setSquare(target);
    }

    return move_legal;
}

void Piece::setSquare(Square *square) {
    if (m_square) {
        m_square->removePiece();
    }
    m_square = square;
    square->putPiece(this);
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