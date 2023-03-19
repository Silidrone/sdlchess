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

    Piece(ChessColor, Board *, const MTexture &, Square * = nullptr);

    Piece(const Piece &other) = default;

    virtual ~Piece();

    Piece &operator=(const Piece &) = default;

    bool fide12(Square *);

    bool fide31(Square *);

    void attack_squares();

    void unattack_squares();

    virtual std::vector<Square *> attacked_squares() = 0;

    virtual bool can_move_to_attacked(Square *) = 0;

    virtual std::vector<Square *> moveable_squares(std::vector<Square *> &);

    bool fide39();

    bool move(Square *, bool = false, std::function<Piece*(Pawn*)> = {});

    virtual void post_move_f(Square *);

    virtual std::string move_log(Square *, bool);

    virtual std::function<void()> move_without_rules(Square *);

    void setSquare(Square *);

    void setPosition(int, int);

    void resetPosition();

    void render();

    Square *getSquare() const;

    void setRenderPriority(int);

    int getRenderPriority() const;

    FDirector getFDirector() const;

    void removeFromBoard();

    bool hasMoved() const;

protected:
    Square *m_square;
    Board *m_board;
    MTexture m_texture;
    int m_render_priority = 0; //TODO check if it is UB to assign this to Piece::DEFAULT_RENDER_PRIORITY if not then do it
    SDL_Rect m_destination{};
    FDirector m_fDirector;
    std::vector<Square *> m_squares_attacked;
    bool m_moved;
};

#endif //CHESS_PIECE_H
