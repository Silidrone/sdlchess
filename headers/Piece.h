#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <string>
#include <utility>
#include "MTexture.h"
#include "SharedData.h"
#include "ChessColored.h"

class Board;
class Square;

class Piece : public ChessColored {
public:
    Piece(ChessColor, Square *, Board *, const MTexture &);
    Piece(const Piece& other) = default;
    virtual ~Piece() {};

    Piece& operator=(const Piece &) = default;

    virtual void move() = 0;

    void setSquare(Square *);

    void setPosition(int, int);

    std::pair<int, int> getPosition() const;
    void render();

    Square *getSquare() const;
protected:
    Square *m_square;
    Board *m_board;
    MTexture m_texture;
    SDL_Rect m_destination;
};

#endif //CHESS_PIECE_H
