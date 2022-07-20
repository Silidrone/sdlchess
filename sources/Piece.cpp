#include "../headers/Piece.h"
#include "../headers/Square.h"

Piece::Piece(ChessColor c, Square *square, Board *board, const MTexture &texture)
        : ChessColored(c), m_board(board), m_texture(texture) {
    setSquare(square);
}

void Piece::setSquare(Square *square) {
    if (m_square) {
        m_square->setPiece(nullptr);
    }
    m_square = square;
    square->setPiece(this);
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