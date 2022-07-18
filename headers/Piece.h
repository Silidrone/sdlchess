#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <string>
#include <utility>
#include "MTexture.h"
#include "SharedData.h"
#include "ChessColored.h"
#include "Square.h"

class Board;

class Piece : public ChessColored {
public:
    Piece(ChessColor c, Square *square, Board *board, const MTexture &texture)
            : ChessColored(c), m_square(square), m_board(board), m_texture(texture) {}
    Piece(const Piece& other) = default;
    virtual ~Piece() = default;

    Piece& operator=(const Piece &) = default;

    virtual void move() = 0;

    void render() {
        m_texture.render(m_square->getDestination());
    }

protected:
    Square *m_square;
    Board *m_board;
    MTexture m_texture;
};

#endif //CHESS_PIECE_H
