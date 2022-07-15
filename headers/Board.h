#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <string>
#include "MTexture.h"
#include "SharedData.h"
#include <vector>
#include "Piece.h"

class Board {
public:
    Board(std::string w_texture_path, std::string b_texture_path) {
        SDL_Renderer *renderer = SharedData::instance().getRenderer();
        MTexture w_texture(renderer);
        MTexture b_texture(renderer);
        w_texture.loadFromFile(std::move(w_texture_path));
        b_texture.loadFromFile(std::move(b_texture_path));
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
