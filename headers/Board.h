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

class Board {
public:
    static const int ROW_SQUARE_COUNT = 8;
    static const int COLUMN_SQUARE_COUNT = 8;
    static const int TOTAL_SQUARE_COUNT = 64;

    Board(const std::string &w_texture_path, const std::string &b_texture_path) : m_pieces() {
        const int square_width = SharedData::instance().SCREEN_WIDTH / ROW_SQUARE_COUNT;
        const int square_height = SharedData::instance().SCREEN_HEIGHT / COLUMN_SQUARE_COUNT;
        SDL_Renderer *renderer = SharedData::instance().getRenderer();
        MTexture w_square_texture(renderer, w_texture_path);
        MTexture b_square_texture(renderer, b_texture_path);
        bool square_white = true;
        m_pieces = {
                //black pieces
                new Rook(ChessColor::BLACK, this),
                new Knight(ChessColor::BLACK, this),
                new Bishop(ChessColor::BLACK, this),
                new Queen(ChessColor::BLACK, this),
                new King(ChessColor::BLACK, this),
                new Bishop(ChessColor::BLACK, this),
                new Knight(ChessColor::BLACK, this),
                new Rook(ChessColor::BLACK, this),
                new Pawn(ChessColor::BLACK, this),

                //white pieces
                new Rook(ChessColor::BLACK, this),

        };

        bool piece_white = false;
        for()

        for (int i = 0; i < TOTAL_SQUARE_COUNT; i++, square_white = !square_white) {
            int current_row = static_cast<int>(floor(i / ROW_SQUARE_COUNT));
            MTexture square_texture(renderer);
            if (current_row > 0 && (i % ROW_SQUARE_COUNT) == 0) {
                square_texture = m_squares.back().getTexture();
                square_white = !square_white;
            } else {
                square_texture = square_white ? w_square_texture : b_square_texture;
            }

            if (current_row == 7) {
                piece_white = true;
            }

            m_squares.emplace_back(square_white ? ChessColor::WHITE : ChessColor::BLACK, square_texture,
                                   SDL_Rect{(i % ROW_SQUARE_COUNT) * square_width,
                                            current_row * square_height, square_width,
                                            square_height},
                                   new Piece(piece_white ? ChessColor::WHITE : ChessColor::BLACK, this,
                                             piece_textures[static_cast<int>(piece_white)][i % ROW_SQUARE_COUNT]));
        }
    }

    ~Board() {
        for(auto &piece : m_pieces) {
            delete piece;
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
    std::vector<Square> m_squares;
    std::vector<Piece *> m_pieces;
};

#endif //CHESS_BOARD_H
