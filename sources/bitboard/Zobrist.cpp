#include "../../headers/bitboard/Zobrist.h"
#include <random>

namespace Bitboard {

// Static member definitions
std::array<std::array<std::array<uint64_t, SQUARE_COUNT>, COLOR_COUNT>, PIECE_TYPE_COUNT> Zobrist::s_pieceKeys;
uint64_t Zobrist::s_sideKey;
std::array<uint64_t, 16> Zobrist::s_castlingKeys;
std::array<uint64_t, 8> Zobrist::s_enPassantKeys;
bool Zobrist::s_initialized = false;

void Zobrist::initialize() {
    if (s_initialized) return;

    // Use a fixed seed for reproducibility
    std::mt19937_64 rng(0x1234567890ABCDEFULL);

    // Generate piece-square keys
    for (int pt = 0; pt < PIECE_TYPE_COUNT; ++pt) {
        for (int c = 0; c < COLOR_COUNT; ++c) {
            for (int sq = 0; sq < SQUARE_COUNT; ++sq) {
                s_pieceKeys[pt][c][sq] = rng();
            }
        }
    }

    // Generate side to move key
    s_sideKey = rng();

    // Generate castling keys
    for (int i = 0; i < 16; ++i) {
        s_castlingKeys[i] = rng();
    }

    // Generate en passant file keys
    for (int file = 0; file < 8; ++file) {
        s_enPassantKeys[file] = rng();
    }

    s_initialized = true;
}

uint64_t Zobrist::pieceSquare(PieceType pt, Color c, Square sq) {
    return s_pieceKeys[pt][c][sq];
}

uint64_t Zobrist::sideToMove() {
    return s_sideKey;
}

uint64_t Zobrist::castling(CastlingRights cr) {
    return s_castlingKeys[cr];
}

uint64_t Zobrist::enPassant(Square sq) {
    // Only the file matters for en passant
    return s_enPassantKeys[fileOf(sq)];
}

} // namespace Bitboard
