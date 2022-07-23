#include "../headers/Knight.h"
#include "../headers/HelperFunctions.h"

std::vector<Square *> Knight::moveable_squares(Square *) {
    return HelperFunctions::get_squares_in_fdirections(m_board, m_square, {
            [](std::pair<int, int> indices) {
                return FDirections::up(
                        FDirections::up(FDirections::left(indices)));
            },
            [](std::pair<int, int> indices) {
                return FDirections::up(
                        FDirections::up(
                                FDirections::right(indices)));
            },
            [](std::pair<int, int> indices) {
                return FDirections::down(
                        FDirections::down(FDirections::left(indices)));
            },
            [](std::pair<int, int> indices) {
                return FDirections::down(
                        FDirections::down(FDirections::right(indices)));
            },
            [](std::pair<int, int> indices) {
                return FDirections::up(FDirections::left(FDirections::left(indices)));
            },
            [](std::pair<int, int> indices) {
                return FDirections::up(FDirections::right(FDirections::right(indices)));
            },
            [](std::pair<int, int> indices) {
                return FDirections::down(FDirections::left(FDirections::left(indices)));
            },
            [](std::pair<int, int> indices) {
                return FDirections::down(FDirections::right(FDirections::right(indices)));
            },
    }, true);
}