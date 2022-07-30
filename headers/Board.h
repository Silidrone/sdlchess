#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <string>
#include "MTexture.h"
#include "SharedData.h"
#include <vector>
#include "Piece.h"
#include "Square.h"
#include "King.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "Pawn.h"
#include <cmath>
#include <functional>

class Board {
private:
    void swapSquares(Square *&, Square *&);

public:
    static const int ROW_SQUARE_COUNT = 8;
    static const int COLUMN_SQUARE_COUNT = 8;
    static constexpr float PIECES_SCALE = 1.0f;

    Board(const std::string &w_texture_path, const std::string &b_texture_path);

    ~Board() = default;

    void render();

    void rotate180();

    std::vector<Square *> get_squares_in_fdirection(Square *, FDirection, bool = false);

    bool coordinateIsValid(std::string);

    Square *get_square_by_coordinate(const std::string &);

    Square *get_square_by_screen_position(int, int);

protected:
    std::vector<Square *> m_squares{};
    std::vector<Piece *> m_pieces{};
    std::vector<std::pair<SDL_Rect, MTexture>> m_coordinate_textures;
};

#endif //CHESS_BOARD_H
