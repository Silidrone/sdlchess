#ifndef CHESS_FDIRECTIONS_H
#define CHESS_FDIRECTIONS_H

#include <utility>

namespace FDirection {
    std::pair<int, int> up(std::pair<int, int> pos) {
        return {pos.first, pos.second - 1};
    }

    std::pair<int, int> down(std::pair<int, int> pos) {
        return {pos.first, pos.second + 1};
    }

    std::pair<int, int> left(std::pair<int, int> pos) {
        return {pos.first - 1, pos.second};
    }

    std::pair<int, int> right(std::pair<int, int> pos) {
        return {pos.first + 1, pos.second};
    }

    std::pair<int, int> upleft(std::pair<int, int> pos) {
        return left(up(pos));
    }

    std::pair<int, int> upright(std::pair<int, int> pos) {
        return right(up(pos));
    }

    std::pair<int, int> downleft(std::pair<int, int> pos) {
        return left(down(pos));
    }

    std::pair<int, int> downright(std::pair<int, int> pos) {
        return right(down(pos));
    }
};

#endif //CHESS_FDIRECTIONS_H
