#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <string>
#include "MTexture.h"
#include "SharedData.h"
#include <vector>
#include "Piece.h"

class Board {
public:
    static const int ROW_SQUARE_COUNT = 8;
    static const int COLUMN_SQUARE_COUNT = 8;
    static const int TOTAL_SQUARE_COUNT = 64;

    Board(std::string w_texture_path, std::string b_texture_path) {
        int square_width = SharedData::instance().SCREEN_WIDTH / ROW_SQUARE_COUNT;
        int square_height = SharedData::instance().SCREEN_HEIGHT / COLUMN_SQUARE_COUNT;
        SDL_Renderer *renderer = SharedData::instance().getRenderer();
//        MTexture w_texture(renderer);
        MTexture b_texture(renderer);
//        w_texture.loadFromFile(std::move(w_texture_path));
        b_texture.loadFromFile(std::move(b_texture_path));
//        m_squares.push_back(w_texture);
        auto b_rect = b_texture.getDestinationRect();
        b_texture.setDestinationRect({b_rect.x, b_rect.y, square_width, square_height});
        m_squares.push_back(b_texture);
    }

    void render() {
        for (auto &square: m_squares) {
            square.render();
        }

        for(auto &piece : m_pieces) {
            piece.render();
        }
    }

protected:
    std::vector<MTexture> m_squares;
    std::vector<Piece> m_pieces;
};

#endif //CHESS_BOARD_H
