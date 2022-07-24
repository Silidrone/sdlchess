#ifndef CHESS_FDIRECTOR_H
#define CHESS_FDIRECTOR_H

#include <utility>
#include <functional>
#include "ChessColored.h"

class FDirector {
public:
    FDirector(ChessColor);

    std::pair<int, int> up(std::pair<int, int>);

    std::pair<int, int> down(std::pair<int, int>);

    std::pair<int, int> left(std::pair<int, int>);

    std::pair<int, int> right(std::pair<int, int>);

    std::pair<int, int> upleft(std::pair<int, int>);

    std::pair<int, int> upright(std::pair<int, int>);

    std::pair<int, int> downleft(std::pair<int, int>);

    std::pair<int, int> downright(std::pair<int, int>);

    std::pair<int, int> upupleft(std::pair<int, int>);

    std::pair<int, int> upupright(std::pair<int, int>);

    std::pair<int, int> downdownleft(std::pair<int, int>);

    std::pair<int, int> downdownright(std::pair<int, int>);

    std::pair<int, int> upleftleft(std::pair<int, int>);

    std::pair<int, int> uprightright(std::pair<int, int>);

    std::pair<int, int> downleftleft(std::pair<int, int>);

    std::pair<int, int> downrightright(std::pair<int, int>);
protected:
    ChessColor m_color;
};

using FDirection = std::pair<int, int> (FDirector::*)(std::pair<int, int>);

#endif //CHESS_FDIRECTOR_H
