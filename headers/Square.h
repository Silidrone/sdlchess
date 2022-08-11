#ifndef CHESS_SQUARE_H
#define CHESS_SQUARE_H

#include "MTexture.h"
#include "ChessColored.h"
#include "FDirector.h"

class Piece;

// TODO add Board to the ctor of this class and remove from Piece. Logically, a square belongs to a board and a piece belongs to a square
class Square : public ChessColored {
public:
    Square(std::string, ChessColor, const MTexture &, SDL_Rect);
    ~Square() = default;

    void render();

    MTexture getTexture() const;

    void putPiece(Piece *);

    void removePiece();

    Piece *getPiece() const;

    void setDestination(int, int);

    SDL_Rect getDestination() const;

    std::string getCoordinate() const;

    void attack(ChessColor);

    void unattack(ChessColor);

    bool isAttacked(ChessColor) const;

    int getAttackCount(ChessColor) const;
protected:
    MTexture m_texture;
    Piece *m_piece;
    SDL_Rect m_destination;
    int m_white_attack_count;
    int m_black_attack_count;
    // TODO this implies that m_coordinate can be a string of any size, when it must only be 2 characters, this should be changed to char[2]
    std::string m_coordinate;
};

#endif
