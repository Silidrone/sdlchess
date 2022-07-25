#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <string>
#include <utility>
#include <vector>
#include "MTexture.h"
#include "SharedData.h"
#include "ChessColored.h"
#include "FDirector.h"

class Board;

class Square;

using DirectionalSquares = std::pair<Direction, std::vector<Square *>>;

class Piece : public ChessColored {
public:
    Piece(ChessColor, Square *, Board *, const MTexture &);

    Piece(const Piece &other) = default;

    virtual ~Piece() {};

    Piece &operator=(const Piece &) = default;

    bool fide12(Square *);

    bool fide31(Square *);

    virtual std::pair<std::vector<Square *>, std::vector<DirectionalSquares>> moveable_squares(Square *) = 0;

    bool fide3p(std::vector<Square *> &, Square *);

    virtual bool fide35(std::vector<DirectionalSquares> &, Square *) = 0;

    bool fide39();

    bool move(Square *);

    void setSquare(Square *);

    void setPosition(int, int);

    void resetPosition();

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
