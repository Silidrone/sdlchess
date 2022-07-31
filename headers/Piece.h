#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <string>
#include <utility>
#include <vector>
#include "MTexture.h"
#include "SharedData.h"
#include "ChessColored.h"
#include "FDirector.h"
#include "../headers/Board.h"

class Square;

class Piece : public ChessColored {
public:
    static const int DEFAULT_RENDER_PRIORITY = 0;

    Piece(ChessColor, Square *, Board *, const MTexture &);

    Piece(const Piece &other) = default;

    virtual ~Piece() {};

    Piece &operator=(const Piece &) = default;

    bool fide12(Square *);

    bool fide31(Square *);

    virtual std::vector<Square *> moveable_squares(Square *) = 0;

    bool fide3p(const std::vector<Square *> &, Square *);

    bool fide39();

    bool move(Square *);

    void setSquare(Square *);

    void setPosition(int, int);

    void resetPosition();

    std::pair<int, int> getPosition() const;

    void render();

    Square *getSquare() const;

    void setRenderPriority(int);

    int getRenderPriority() const;

protected:
    Square *m_square;
    Board *m_board;
    MTexture m_texture;
    int m_render_priority = 0; //TODO check if it is UB to assign this to Piece::DEFAULT_RENDER_PRIORITY if not then do it
    SDL_Rect m_destination{};
};

#endif //CHESS_PIECE_H
