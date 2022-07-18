#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <string>
#include <utility>
#include "MTexture.h"
#include "SharedData.h"
#include "ChessColored.h"

class Board;

class Piece : public ChessColored {
public:
    Piece(ChessColor c, Board *board, const MTexture &texture)
            : ChessColored(c), m_board(board), m_texture(texture) {}
    Piece(const Piece& other) = default;
    virtual ~Piece() = default;

    Piece& operator=(const Piece &) = default;

    virtual bool move() = 0;

    void render(SDL_Rect destination) {
        m_texture.render(destination);
    }

protected:
    Board *m_board;
    MTexture m_texture;
};

#endif //CHESS_PIECE_H
