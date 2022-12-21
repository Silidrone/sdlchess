#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <string>
#include <vector>
#include "MTexture.h"
#include "../headers/FDirector.h"
#include <array>

using DirectionalSquares = std::pair<Direction, std::vector<Square *>>;

class Piece;

class King;

class Rook;

class MoveLogger;

class Queen;

class Pawn;

class Bishop;

class Knight;

class Rook;

class Board {
protected:
    template <class T>
    std::vector<T *> getPieces(ChessColor);
public:
    static const int ROW_SQUARE_COUNT = 8;
    static const int COLUMN_SQUARE_COUNT = 8;
    static constexpr float PIECES_SCALE = 1.0f;

    explicit Board(MoveLogger &);

    ~Board();

    void init(const std::string &, const std::string &);

    void render();

    void updateRenderOrder();

    void rotate180();

    bool legalMoveExists(ChessColor);

    std::vector<Square *> get_squares_in_fdirection(Piece *, FDirection, bool = false);

    std::pair<std::vector<Square *>, std::vector<DirectionalSquares>>
    get_squares_in_fdirections(Piece *, std::vector<std::pair<Direction, FDirection>>, bool = false);

    bool coordinateIsValid(std::string);

    Square *get_square_by_coordinate(const std::string &);

    Square *get_square_by_screen_position(int, int);

    void removePiece(Piece *);
    void unRemovePiece(Piece *);

    void addPiece(Piece *);

    void updateAttackedSquares();

    King *getKing(ChessColor);
    Queen *getQueen(ChessColor);
    std::vector<Pawn*> getPawns(ChessColor);
    std::vector<Bishop*> getBishops(ChessColor);
    std::vector<Knight*> getKnights(ChessColor);
    std::vector<Rook*> getRooks(ChessColor);

    bool isGameOver(ChessColor, ChessColor);

    MoveLogger &getMoveLogger() const;
protected:
    std::vector<Square *> m_squares{};
    std::vector<Piece *> m_pieces{};
    std::vector<Piece *> m_removed_pieces{};
    std::array<std::vector<std::pair<SDL_Rect, MTexture>>, 2> m_coordinate_textures{};
    MoveLogger &m_moveLogger;
};

#endif //CHESS_BOARD_H
