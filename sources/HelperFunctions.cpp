#include "../headers/HelperFunctions.h"

std::string HelperFunctions::get_algebraic_notation(char piece_letter, const std::string& current_coordinate, bool captured) {
    if (captured) {
        return std::string() + piece_letter + 'x' + current_coordinate;
    } else {
        return piece_letter + current_coordinate;
    }
}