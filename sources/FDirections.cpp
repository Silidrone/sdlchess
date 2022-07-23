#include "../headers/FDirections.h"

std::pair<int, int> FDirections::up(std::pair<int, int> pos) {
    return {pos.first, pos.second - 1};
}

std::pair<int, int> FDirections::down(std::pair<int, int> pos) {
    return {pos.first, pos.second + 1};
}

std::pair<int, int> FDirections::left(std::pair<int, int> pos) {
    return {pos.first - 1, pos.second};
}

std::pair<int, int> FDirections::right(std::pair<int, int> pos) {
    return {pos.first + 1, pos.second};
}

std::pair<int, int> FDirections::upleft(std::pair<int, int> pos) {
    return left(up(pos));
}

std::pair<int, int> FDirections::upright(std::pair<int, int> pos) {
    return right(up(pos));
}

std::pair<int, int> FDirections::downleft(std::pair<int, int> pos) {
    return left(down(pos));
}

std::pair<int, int> FDirections::downright(std::pair<int, int> pos) {
    return right(down(pos));
}
