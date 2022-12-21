#ifndef CHESS_FDIRECTOR_H
#define CHESS_FDIRECTOR_H

#include <string>
#include <vector>
#include <functional>
#include "ChessColored.h"
#include <map>

class Square;

class Piece;

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
    UP_UP_LEFT,
    UP_UP_RIGHT,
    UP_LEFT_LEFT,
    UP_RIGHT_RIGHT,
    DOWN_DOWN_LEFT,
    DOWN_DOWN_RIGHT,
    DOWN_LEFT_LEFT,
    DOWN_RIGHT_RIGHT
};


class FDirector {
protected:
    ChessColor m_color;
public:
    explicit FDirector(ChessColor);

    std::string up(std::string);

    std::string down(std::string);

    std::string left(std::string);

    std::string right(std::string);

    std::string up_left(std::string);

    std::string up_right(std::string);

    std::string down_left(std::string);

    std::string down_right(std::string);

    std::string up_up_left(std::string);

    std::string up_up_right(std::string);

    std::string down_down_left(std::string);

    std::string down_down_right(std::string);

    std::string up_left_left(std::string);

    std::string up_right_right(std::string);

    std::string down_left_left(std::string);

    std::string down_right_right(std::string);
};

using FDirection = std::string(FDirector::*)(std::string);

#endif //CHESS_FDIRECTOR_H