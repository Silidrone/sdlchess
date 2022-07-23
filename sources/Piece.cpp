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
    return (target->getPiece()->getColor() != this->getColor());
}

bool Piece::fide3p(std::vector<Square *> &legal_squares, Square *target) {
    return find_if(legal_squares.begin(), legal_squares.end(),
                   [&target](Square *s) { return s->getCoordinate() == target->getCoordinate(); }) !=
           legal_squares.end();
}

bool Piece::fide35(std::vector<Square *> &squares) {
    return !HelperFunctions::squaresHaveAnyPieces(squares);
}

//TODO finish the implementation of fide39 rule
bool Piece::fide39() {
    return true;
}

bool Piece::move(Square *target) {
    std::vector<Square *> legal_squares = moveable_squares(target);
    return fide12(target) && fide31(target) && fide3p(legal_squares, target) && fide35(legal_squares) && fide39();
}

void Piece::setSquare(Square *square) {
    if (m_square) {
        m_square->removePiece();
    }
    m_square = square;
    square->putPiece(this);
    m_destination = square->getDestination();
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