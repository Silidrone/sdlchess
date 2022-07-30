#include "../headers/FDirector.h"

ChessColor FDirector::color;

std::string FDirector::up(std::string coordinate) {
    return std::string() + coordinate[0] + static_cast<char>(coordinate[1] + (color == ChessColor::WHITE ? 1 : -1));
}

std::string FDirector::down(std::string coordinate) {
    return std::string() + coordinate[0] + static_cast<char>(coordinate[1] - (color == ChessColor::WHITE ? 1 : -1));
}

std::string FDirector::left(std::string coordinate) {
    return std::string() + static_cast<char>(coordinate[0] - (color == ChessColor::WHITE ? 1 : -1)) + coordinate[1];
}

std::string FDirector::right(std::string coordinate) {
    return std::string() + static_cast<char>(coordinate[0] + (color == ChessColor::WHITE ? 1 : -1)) + coordinate[1];
}

std::string FDirector::up_left(std::string coordinate) {
    return left(up(std::move(coordinate)));
}

std::string FDirector::up_right(std::string coordinate) {
    return right(up(std::move(coordinate)));
}

std::string FDirector::down_left(std::string coordinate) {
    return left(down(std::move(coordinate)));
}

std::string FDirector::down_right(std::string coordinate) {
    return right(down(std::move(coordinate)));
}

std::string FDirector::up_up_left(std::string coordinate) {
    return left(up(up(std::move(coordinate))));
}

std::string FDirector::up_up_right(std::string coordinate) {
    return right(up(up(std::move(coordinate))));
}

std::string FDirector::down_down_left(std::string coordinate) {
    return left(down(down(std::move(coordinate))));
}

std::string FDirector::down_down_right(std::string coordinate) {
    return right(down(down(std::move(coordinate))));
}

std::string FDirector::up_left_left(std::string coordinate) {
    return left(left(up(std::move(coordinate))));
}

std::string FDirector::up_right_right(std::string coordinate) {
    return right(right(up(std::move(coordinate))));
}

std::string FDirector::down_left_left(std::string coordinate) {
    return left(left(down(std::move(coordinate))));
}

std::string FDirector::down_right_right(std::string coordinate) {
    return right(right(down(std::move(coordinate))));
}