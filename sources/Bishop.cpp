#include "../headers/Bishop.h"

std::vector<std::pair<Direction, FDirection>> Bishop::getDirections() {
    return {{Direction::UP_LEFT,    FDirector::up_left},
            {Direction::UP_RIGHT,   FDirector::up_right},
            {Direction::DOWN_LEFT,  FDirector::down_left},
            {Direction::DOWN_RIGHT, FDirector::down_right}};
}