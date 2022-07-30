#include "../headers/Square.h"
#include "../headers/Piece.h"

Square::Square(std::string coordinate, ChessColor c, const MTexture &t, SDL_Rect dest)
        : m_coordinate(std::move(coordinate)), ChessColored(c), m_texture(t), m_destination(dest),
          m_piece(nullptr) {
}

void Square::render() {
    m_texture.render(m_destination);
}

MTexture Square::getTexture() const {
    return m_texture;
}

void Square::putPiece(Piece *p) {
    if (m_piece) {
        delete m_piece;
        m_piece = nullptr;
    }
    m_piece = p;
}

void Square::removePiece() {
    m_piece = nullptr;
}

Piece *Square::getPiece() const {
    return m_piece;
}

void Square::setDestination(int x, int y) {
    m_destination = {x, y, m_destination.w, m_destination.h};
    if (m_piece) m_piece->resetPosition();
}

SDL_Rect Square::getDestination() const {
    return m_destination;
}

std::string Square::getCoordinate() const {
    return m_coordinate;
}
