#include "../headers/Rook.h"

std::vector<std::pair<Direction, FDirection>> Rook::getDirections() {
    return {
            {Direction::UP,    FDirector::up},
            {Direction::DOWN,  FDirector::down},
            {Direction::LEFT,  FDirector::left},
            {Direction::RIGHT, FDirector::right},
    };
}