#include "../headers/QRBPiece.h"
#include "../headers/Square.h"

QRBPiece::QRBPiece(ChessColor c, Square *s, Board *b, const MTexture &t) : Piece(c, s, b, t) {}

std::vector<Square *>
QRBPiece::squaresBeforeNextPieceInDirection(const DirectionalSquares &directional_squares) {
    auto it = directional_squares.second.begin();
    for (; it != directional_squares.second.end(); it++) {
        Piece *p = (*it)->getPiece();
        if (p != nullptr) {
            if (getColor() != p->getColor()) {
                it++;
            }
            break;
        }
    }

    return std::vector<Square *>(directional_squares.second.begin(), it);
}

std::vector<Square *> QRBPiece::squaresBeforeNextPieceInDirections(const std::vector<DirectionalSquares> &dss) {
    std::vector<Square *> result;
    for (auto &ds: dss) {
        auto squares = squaresBeforeNextPieceInDirection(ds);
        result.insert(result.begin(), squares.begin(), squares.end());
    }

    return result;
}

std::vector<Square *> QRBPiece::moveable_squares(Square *target) {
    return squaresBeforeNextPieceInDirections(m_board->get_squares_in_fdirections(m_square, getDirections()).second);
}