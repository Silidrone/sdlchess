#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <string>
#include <utility>
#include "MTexture.h"
#include "SharedData.h"

enum class Color {
    WHITE,
    BLACK
};

class Board;

class Piece {
public:
    Piece(Board *board, Color c, std::string texture_path, std::string position) : m_board(board), m_color(c),
                                                                                   m_texture(
                                                                                           SharedData::instance().getRenderer()),
                                                                                   m_position(std::move(position)) {
        m_texture.loadFromFile(std::move(texture_path));
    }

    virtual ~Piece() = default;

    virtual bool move() = 0;

    std::string getPosition() const { return m_position; }

    Color getColor() const { return m_color; }

    void render() {
        m_texture.render();
    }
protected:
    Board *m_board;
    MTexture m_texture;
    std::string m_position;
    Color m_color;
};

#endif //CHESS_PIECE_H
