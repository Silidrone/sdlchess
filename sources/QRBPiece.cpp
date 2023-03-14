#include "../headers/QRBPiece.h"
#include "../headers/Square.h"

QRBPiece::QRBPiece(ChessColor c, Board *b, const MTexture &t, Square *s) : Piece(c, b, t, s) {}

QRBPiece::~QRBPiece() {}

std::vector<Square *>
QRBPiece::squaresBeforeNextPieceInDirection(const DirectionalSquares &directional_squares) {
    auto it = directional_squares.second.begin();
    for (; it != directional_squares.second.end(); it++) {
        if ((*it)->getPiece() != nullptr) {
            it++;
            break;
        }
    }

    return std::vector<Square *>(directional_squares.second.begin(), it);
}

std::vector<Square *> QRBPiece::squaresBeforeNextPieceInDirections(const std::vector<DirectionalSquares> &dss) {
    std::vector<Square *> result;
    for (auto &ds: dss) {
        auto squares = squaresBeforeNextPieceInDirection(ds);
        result.insert(result.end(), squares.begin(), squares.end());
    }

    return result;
}

std::vector<Square *> QRBPiece::attacked_squares() {
    return squaresBeforeNextPieceInDirections(m_board->get_squares_in_fdirections(this, getDirections()).second);
}

bool QRBPiece::can_move_to_attacked(Square *s) {
    Piece *p = s->getPiece();
    return p == nullptr || p->getColor() != getColor();
}