#ifndef CHESS_SQUARE_H
#define CHESS_SQUARE_H

#include "MTexture.h"
#include "ChessColored.h"

class Piece;

// TODO add Board to the ctor of this class and remove from Piece. Logically, a square belongs to a board and a piece belongs to a square
class Square : public ChessColored {
public:
    Square(std::string, ChessColor, const MTexture &, SDL_Rect);

    void render();

    MTexture getTexture() const;

    void putPiece(Piece *);

    void removePiece();

    Piece *getPiece() const;

    void setDestination(int, int);

    SDL_Rect getDestination() const;

    std::string getCoordinate() const;

protected:
    MTexture m_texture;
    Piece *m_piece;
    SDL_Rect m_destination;
    // TODO this implies that m_coordinate can be a string of any size, when it must only be 2 characters, this should be changed to char[2]
    std::string m_coordinate;
};

#endif
