#include "../headers/FDirector.h"
#include "../headers/Board.h"
#include "../headers/Square.h"
#include "../headers/Piece.h"

FDirector::FDirector(ChessColor c) : m_color(c) {}

std::string FDirector::up(std::string coordinate) {
    return std::string() + coordinate[0] + static_cast<char>(coordinate[1] + (m_color == ChessColor::WHITE ? 1 : -1));
}

std::string FDirector::down(std::string coordinate) {
    return std::string() + coordinate[0] + static_cast<char>(coordinate[1] - (m_color == ChessColor::WHITE ? 1 : -1));
}

std::string FDirector::left(std::string coordinate) {
    return std::string() + static_cast<char>(coordinate[0] - (m_color == ChessColor::WHITE ? 1 : -1)) + coordinate[1];
}

std::string FDirector::right(std::string coordinate) {
    return std::string() + static_cast<char>(coordinate[0] + (m_color == ChessColor::WHITE ? 1 : -1)) + coordinate[1];
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

Direction FDirector::get_target_direction(Piece *this_piece, Square *target_square) {
    std::string this_coordinate = this_piece->getSquare()->getCoordinate();
    std::string target_coordinate = target_square->getCoordinate();
    bool up = target_coordinate[1] > this_coordinate[1];
    bool same_row = target_coordinate[1] == this_coordinate[1];
    bool left = target_coordinate[0] < this_coordinate[0];
    bool same_column = target_coordinate[0] == this_coordinate[0];

    if (m_color == ChessColor::BLACK) {
        up = !up;
        left = !left;
    }

    if (same_column) {
        if (up) {
            return Direction::UP;
        } else {
            return Direction::DOWN;
        }
    } else if (same_row) {
        if (left) {
            return Direction::LEFT;
        } else {
            return Direction::RIGHT;
        }
    } else if (up && left) {
        return Direction::UP_LEFT;
    } else if (up) {
        return Direction::UP_RIGHT;
    } else if (left) {
        return Direction::DOWN_LEFT;
    } else {
        return Direction::DOWN_RIGHT;
    }
}
