#include "../headers/King.h"
#include "../headers/Square.h"

std::vector<Square *> King::attacked_squares() {
    // TODO finish after adding "attacked" property to a square as King has to be able to know if the squares he can move to are attacked or not
    return {};
}

bool King::can_move_to_attacked(Square *) {
    return false;
}