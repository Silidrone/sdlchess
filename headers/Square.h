#ifndef CHESS_SQUARE_H
#define CHESS_SQUARE_H

#include "MTexture.h"
#include "Piece.h"

class Square : public ChessColored {
public:
    Square(std::string name, ChessColor c, const MTexture &t, SDL_Rect dest) : m_name(
            std::move(name)), ChessColored(c), m_texture(t), m_destination(dest), m_piece(nullptr) {}

    void render() {
        m_texture.render(m_destination);
    }

    MTexture getTexture() const { return m_texture; }

    void setPiece(Piece *p, bool remove_relations = true) {
        if(remove_relations && m_piece) {
            m_piece->setSquare(nullptr);
        }
        m_piece = p;
    }

    Piece *getPiece() const { return m_piece; }

    SDL_Rect getDestination() const { return m_destination; }

protected:
    MTexture m_texture;
    Piece *m_piece;
    SDL_Rect m_destination;
    std::string m_name;
};

#endif
