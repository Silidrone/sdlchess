#ifndef CHESS_SQUARE_H
#define CHESS_SQUARE_H

#include "MTexture.h"
#include "Piece.h"

class Square : public ChessColored {
public:
    Square(std::string coordinate, ChessColor c, const MTexture &t, SDL_Rect dest) : m_coordinate(
            std::move(coordinate)), ChessColored(c), m_texture(t), m_destination(dest), m_piece(nullptr) {}

    void render() {
        m_texture.render(m_destination);
    }

    MTexture getTexture() const { return m_texture; }

    void putPiece(Piece *p) {
        if (m_piece) {
            delete m_piece;
            m_piece = nullptr;
        }
        m_piece = p;
    }

    void removePiece() {
        m_piece = nullptr;
    }

    Piece *getPiece() const { return m_piece; }

    SDL_Rect getDestination() const { return m_destination; }

    std::string getCoordinate() const { return m_coordinate; }
protected:
    MTexture m_texture;
    Piece *m_piece;
    SDL_Rect m_destination;
    std::string m_coordinate;
};

#endif
