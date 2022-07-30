#ifndef CHESS_FDIRECTOR_H
#define CHESS_FDIRECTOR_H

#include <string>
#include <functional>
#include "ChessColored.h"

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

using FDirection = std::function<std::string(std::string)>;

class FDirector {
public:
    static ChessColor color;

    static std::string up(std::string);

    static std::string down(std::string);

    static std::string left(std::string);

    static std::string right(std::string);

    static std::string up_left(std::string);

    static std::string up_right(std::string);

    static std::string down_left(std::string);

    static std::string down_right(std::string);

    static std::string up_up_left(std::string);

    static std::string up_up_right(std::string);

    static std::string down_down_left(std::string);

    static std::string down_down_right(std::string);

    static std::string up_left_left(std::string);

    static std::string up_right_right(std::string);

    static std::string down_left_left(std::string);

    static std::string down_right_right(std::string);
};

#endif //CHESS_FDIRECTOR_H