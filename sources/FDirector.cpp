#include "../headers/FDirector.h"

FDirector::FDirector(ChessColor c) : m_color(c) {}

std::pair<int, int> FDirector::up(std::pair<int, int> pos) {
    return {pos.first + (m_color == ChessColor::WHITE ? 1 : -1), pos.second};
}

std::pair<int, int> FDirector::down(std::pair<int, int> pos) {
    return {pos.first - (m_color == ChessColor::WHITE ? 1 : -1), pos.second};
}

std::pair<int, int> FDirector::left(std::pair<int, int> pos) {
    return {pos.first, pos.second - (m_color == ChessColor::WHITE ? 1 : -1)};
}

std::pair<int, int> FDirector::right(std::pair<int, int> pos) {
    return {pos.first, pos.second + (m_color == ChessColor::WHITE ? 1 : -1)};
}

std::pair<int, int> FDirector::upleft(std::pair<int, int> pos) {
    return left(up(pos));
}

std::pair<int, int> FDirector::upright(std::pair<int, int> pos) {
    return right(up(pos));
}

std::pair<int, int> FDirector::downleft(std::pair<int, int> pos) {
    return left(down(pos));
}

std::pair<int, int> FDirector::downright(std::pair<int, int> pos) {
    return right(down(pos));
}

std::pair<int, int> FDirector::upupleft(std::pair<int, int> indices) {
    return up(up(left(indices)));
}

std::pair<int, int> FDirector::upupright(std::pair<int, int> indices){
    return up(up(right(indices)));
}

std::pair<int, int> FDirector::downdownleft(std::pair<int, int> indices){
    return down(down(left(indices)));
}

std::pair<int, int> FDirector::downdownright(std::pair<int, int> indices){
    return down(down(right(indices)));
}

std::pair<int, int> FDirector::upleftleft(std::pair<int, int> indices){
    return up(left(left(indices)));
}

std::pair<int, int> FDirector::uprightright(std::pair<int, int> indices){
    return up(right(right(indices)));
}

std::pair<int, int> FDirector::downleftleft(std::pair<int, int> indices){
    return down(left(left(indices)));
}

std::pair<int, int> FDirector::downrightright(std::pair<int, int> indices){
    return down(right(right(indices)));
}
