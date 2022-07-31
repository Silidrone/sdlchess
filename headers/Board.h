#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <string>
#include <vector>
#include "MTexture.h"
#include "../headers/FDirector.h"
#include <array>

using DirectionalSquares = std::pair<Direction, std::vector<Square *>>;

class Piece;

class Board {
public:
    static const int ROW_SQUARE_COUNT = 8;
    static const int COLUMN_SQUARE_COUNT = 8;
    static constexpr float PIECES_SCALE = 1.0f;

    Board(const std::string &w_texture_path, const std::string &b_texture_path);

    ~Board() = default;

    void render();

    void rotate180();

    std::vector<Square *> get_squares_in_fdirection(Square *, FDirection, bool = false);

    std::pair<std::vector<Square *>, std::vector<DirectionalSquares>>
    get_squares_in_fdirections(Square *, std::vector<std::pair<Direction, FDirection>>, bool = false);

    bool coordinateIsValid(std::string);

    Square *get_square_by_coordinate(const std::string &);

    Square *get_square_by_screen_position(int, int);

protected:
    std::vector<Square *> m_squares{};
    std::vector<Piece *> m_pieces{};
    std::array<std::vector<std::pair<SDL_Rect, MTexture>>, 2> m_coordinate_textures;
};

#endif //CHESS_BOARD_H
