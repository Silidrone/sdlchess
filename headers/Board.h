#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <string>
#include "MTexture.h"
#include "SharedData.h"
#include <vector>
#include "Piece.h"
#include <cmath>

class Board {
public:
    static const int ROW_SQUARE_COUNT = 8;
    static const int COLUMN_SQUARE_COUNT = 8;
    static const int TOTAL_SQUARE_COUNT = 64;

    Board(const std::string &w_texture_path, const std::string &b_texture_path) {
        const int square_width = SharedData::instance().SCREEN_WIDTH / ROW_SQUARE_COUNT;
        const int square_height = SharedData::instance().SCREEN_HEIGHT / COLUMN_SQUARE_COUNT;
        SDL_Renderer *renderer = SharedData::instance().getRenderer();
        MTexture w_square_texture(renderer);
        w_square_texture.loadFromFile(w_texture_path);
        MTexture b_square_texture(renderer);
        b_square_texture.loadFromFile(b_texture_path);
        bool white = false;
        for (int i = 0; i < TOTAL_SQUARE_COUNT; i++, white = !white) {
            int current_row = static_cast<int>(floor(i / ROW_SQUARE_COUNT));
            MTexture square_texture(renderer);
            if (current_row > 0 && (i % ROW_SQUARE_COUNT) == 0) {
                square_texture = m_squares.back();
                white = !white;
            } else {
                square_texture = white ? b_square_texture : w_square_texture;
            }
            square_texture.setDestinationRect(
                    {(i % ROW_SQUARE_COUNT) * square_width,
                     current_row * square_height, square_width,
                     square_height});
            m_squares.push_back(square_texture);
        }
    }

    void render() {
        for (auto &square: m_squares) {
            square.render();
        }

        for (auto &piece: m_pieces) {
            piece.render();
        }
    }

protected:
    std::vector<MTexture> m_squares;
    std::vector<Piece> m_pieces;
};

#endif //CHESS_BOARD_H
