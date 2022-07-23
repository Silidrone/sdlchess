#ifndef CHESS_FDIRECTIONS_H
#define CHESS_FDIRECTIONS_H

#include <utility>
#include <functional>

using FDirection = std::function<std::pair<int, int>(std::pair<int, int>)>;

namespace FDirections {
    std::pair<int, int> up(std::pair<int, int>);

    std::pair<int, int> down(std::pair<int, int>);

    std::pair<int, int> left(std::pair<int, int>);

    std::pair<int, int> right(std::pair<int, int>);

    std::pair<int, int> upleft(std::pair<int, int>);

    std::pair<int, int> upright(std::pair<int, int>);

    std::pair<int, int> downleft(std::pair<int, int>);

    std::pair<int, int> downright(std::pair<int, int>);
};

#endif //CHESS_FDIRECTIONS_H
