#ifndef CHESS_SQUARE_H
#define CHESS_SQUARE_H

#include "MTexture.h"

class Piece;

class Square : public ChessColored {
public:
    Square(std::string name, ChessColor c, const MTexture &t, SDL_Rect dest) : m_name(
            std::move(name)), ChessColored(c), m_texture(t), m_destination(dest), m_piece(nullptr), m_attacked(false) {}

    void render() {
        m_texture.render(m_destination);
    }

    MTexture getTexture() const { return m_texture; }

    void setPiece(Piece *p) { m_piece = p; }

    void removePiece() { m_piece = nullptr; }

    void setAttacked(bool attacked) { m_attacked = attacked; }

    bool is_occupied() { return m_piece != nullptr; }

    bool is_attacked() { return m_attacked; }

    SDL_Rect getDestination() const { return m_destination; }
    
protected:
    MTexture m_texture;
    Piece *m_piece;
    bool m_attacked;
    SDL_Rect m_destination;
    std::string m_name;
};

#endif
