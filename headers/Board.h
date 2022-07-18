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
    static const int TOTAL_SQUARE_COUNT = ROW_SQUARE_COUNT * COLUMN_SQUARE_COUNT;

    Board(const std::string &w_texture_path, const std::string &b_texture_path) : m_pieces() {
        const int square_width = SharedData::instance().SCREEN_WIDTH / ROW_SQUARE_COUNT;
        const int square_height = SharedData::instance().SCREEN_HEIGHT / COLUMN_SQUARE_COUNT;
        SDL_Renderer *renderer = SharedData::instance().getRenderer();
        MTexture w_square_texture(renderer, w_texture_path);
        MTexture b_square_texture(renderer, b_texture_path);

        ChessColor initializationSide = ChessColor::WHITE;

        bool square_white = static_cast<bool>(initializationSide);
        for (int i = 0; i < ROW_SQUARE_COUNT; i++) {
            for (int j = 0; j < COLUMN_SQUARE_COUNT; j++, square_white = !square_white) {
                MTexture square_texture(renderer);
                if (i > 0 && j == 0) {
                    square_texture = m_squares.back()->getTexture();
                    square_white = !square_white;
                } else {
                    square_texture = square_white ? w_square_texture : b_square_texture;
                }

                m_squares.push_back(
                        new Square(std::string() + (static_cast<char>('a' + i)) + static_cast<char>('1' + j),
                                   static_cast<ChessColor>(square_white), square_texture,
                                   SDL_Rect{j * square_width, i * square_height, square_width, square_height}));
            }
        }

        ChessColor oppositeInitializationSide = static_cast<ChessColor>(!static_cast<bool>(initializationSide));
        m_pieces = {
                //black pieces
                new Rook(oppositeInitializationSide, m_squares[0], this),
                new Knight(oppositeInitializationSide, m_squares[1], this),
                new Bishop(oppositeInitializationSide, m_squares[2], this),
                new Queen(oppositeInitializationSide, m_squares[3], this),
                new King(oppositeInitializationSide, m_squares[4], this),
                new Bishop(oppositeInitializationSide, m_squares[5], this),
                new Knight(oppositeInitializationSide, m_squares[6], this),
                new Rook(oppositeInitializationSide, m_squares[7], this),
                new Pawn(oppositeInitializationSide, m_squares[8], this),
                new Pawn(oppositeInitializationSide, m_squares[9], this),
                new Pawn(oppositeInitializationSide, m_squares[10], this),
                new Pawn(oppositeInitializationSide, m_squares[11], this),
                new Pawn(oppositeInitializationSide, m_squares[12], this),
                new Pawn(oppositeInitializationSide, m_squares[13], this),
                new Pawn(oppositeInitializationSide, m_squares[14], this),
                new Pawn(oppositeInitializationSide, m_squares[15], this),

                //white pieces
                new Rook(initializationSide, m_squares[63], this),
                new Knight(initializationSide, m_squares[62], this),
                new Bishop(initializationSide, m_squares[61], this),
                new King(initializationSide, m_squares[60], this),
                new Queen(initializationSide, m_squares[59], this),
                new Bishop(initializationSide, m_squares[58], this),
                new Knight(initializationSide, m_squares[57], this),
                new Rook(initializationSide, m_squares[56], this),
                new Pawn(initializationSide, m_squares[55], this),
                new Pawn(initializationSide, m_squares[54], this),
                new Pawn(initializationSide, m_squares[53], this),
                new Pawn(initializationSide, m_squares[52], this),
                new Pawn(initializationSide, m_squares[51], this),
                new Pawn(initializationSide, m_squares[50], this),
                new Pawn(initializationSide, m_squares[49], this),
                new Pawn(initializationSide, m_squares[48], this),
        };



    }

    ~Board() {
        for (auto &piece: m_pieces) {
            delete piece;
        }
    }

    void render() {
        for (auto &square: m_squares) {
            square->render();
        }

        for (auto &piece: m_pieces) {
            piece->render();
        }
    }

protected:
    std::vector<Square *> m_squares;
    std::vector<Piece *> m_pieces;
};

#endif //CHESS_BOARD_H
