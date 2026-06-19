#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "BitboardTypes.h"
#include <array>

namespace Bitboard {

// Zobrist hashing for position identification
// Uses random 64-bit numbers XORed together based on position features
class Zobrist {
public:
    // Initialize all Zobrist keys (call once at startup)
    static void initialize();

    // Get key for piece on square
    static uint64_t pieceSquare(PieceType pt, Color c, Square sq);

    // Get key for side to move (XOR when black to move)
    static uint64_t sideToMove();

    // Get key for castling rights
    static uint64_t castling(CastlingRights cr);

    // Get key for en passant file (only if en passant is possible)
    static uint64_t enPassant(Square sq);

private:
    // Keys for pieces: [piece_type][color][square]
    static std::array<std::array<std::array<uint64_t, SQUARE_COUNT>, COLOR_COUNT>, PIECE_TYPE_COUNT> s_pieceKeys;

    // Key for black to move
    static uint64_t s_sideKey;

    // Keys for castling rights (one for each possible combination)
    static std::array<uint64_t, 16> s_castlingKeys;

    // Keys for en passant file (8 files)
    static std::array<uint64_t, 8> s_enPassantKeys;

    static bool s_initialized;
};

} // namespace Bitboard

#endif // ZOBRIST_H
