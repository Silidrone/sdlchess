#ifndef MAGIC_BITBOARDS_H
#define MAGIC_BITBOARDS_H

#include "BitboardTypes.h"
#include "BitboardUtils.h"
#include <array>

namespace Bitboard {

// Magic structure for a single square
struct Magic {
    Bitboard mask;      // Relevant occupancy mask (excluding edges)
    Bitboard magic;     // Magic multiplier for this square
    Bitboard* attacks;  // Pointer into the attack table
    int shift;          // Right shift amount (64 - number of relevant bits)

    // Compute index into attack table using magic multiplication
    unsigned index(Bitboard occupied) const {
        return static_cast<unsigned>(((occupied & mask) * magic) >> shift);
    }
};

class MagicBitboards {
public:
    // Initialize all attack tables (call once at startup)
    static void initialize();

    // Check if initialized
    static bool isInitialized() { return s_initialized; }

    // Get attacks for sliding pieces given occupancy
    static Bitboard bishopAttacks(Square sq, Bitboard occupied);
    static Bitboard rookAttacks(Square sq, Bitboard occupied);
    static Bitboard queenAttacks(Square sq, Bitboard occupied);

    // Get attacks for non-sliding pieces (no occupancy needed)
    static Bitboard knightAttacks(Square sq);
    static Bitboard kingAttacks(Square sq);
    static Bitboard pawnAttacks(Color c, Square sq);

    // Get all pawn attacks for a color
    static Bitboard pawnAttacks(Color c, Bitboard pawns);

    // Line between two squares (for pin detection)
    static Bitboard lineBetween(Square sq1, Square sq2);

    // Line through two squares (extends beyond both)
    static Bitboard lineThrough(Square sq1, Square sq2);

    // Get pseudo-attacks (attacks on empty board) for sliding pieces
    static Bitboard pseudoBishopAttacks(Square sq);
    static Bitboard pseudoRookAttacks(Square sq);

private:
    // Initialization helpers
    static void initKnightAttacks();
    static void initKingAttacks();
    static void initPawnAttacks();
    static void initMagics(bool isBishop);
    static void initLines();

    // Compute sliding attacks using classical approach (for magic generation)
    static Bitboard computeSlidingAttacks(Square sq, Bitboard occupied, bool isBishop);

    // Compute the relevant occupancy mask for a square
    static Bitboard computeRelevantOccupancy(Square sq, bool isBishop);

    // Find a magic number for a square
    static Bitboard findMagic(Square sq, bool isBishop);

    // Random number generator for magic finding
    static uint64_t random64();
    static uint64_t random64Sparse();

    // Attack tables for non-sliding pieces
    static std::array<Bitboard, 64> s_knightAttacks;
    static std::array<Bitboard, 64> s_kingAttacks;
    static std::array<std::array<Bitboard, 64>, 2> s_pawnAttacks; // [color][square]

    // Pseudo attacks (attacks on empty board)
    static std::array<Bitboard, 64> s_pseudoBishopAttacks;
    static std::array<Bitboard, 64> s_pseudoRookAttacks;

    // Magic structures for sliding pieces
    static std::array<Magic, 64> s_bishopMagics;
    static std::array<Magic, 64> s_rookMagics;

    // Attack tables for sliding pieces (indexed by magic hash)
    // Bishop needs up to 512 entries per square (9 bits max)
    // Rook needs up to 4096 entries per square (12 bits max)
    static std::array<Bitboard, 0x1480> s_bishopTable;  // Total size for all squares
    static std::array<Bitboard, 0x19000> s_rookTable;   // Total size for all squares

    // Line masks for pin/check detection
    static std::array<std::array<Bitboard, 64>, 64> s_lineBetween;
    static std::array<std::array<Bitboard, 64>, 64> s_lineThrough;

    static bool s_initialized;
    static uint64_t s_rngState;
};

// Convenience inline functions for direct access

inline Bitboard bishopAttacks(Square sq, Bitboard occ) {
    return MagicBitboards::bishopAttacks(sq, occ);
}

inline Bitboard rookAttacks(Square sq, Bitboard occ) {
    return MagicBitboards::rookAttacks(sq, occ);
}

inline Bitboard queenAttacks(Square sq, Bitboard occ) {
    return MagicBitboards::queenAttacks(sq, occ);
}

inline Bitboard knightAttacks(Square sq) {
    return MagicBitboards::knightAttacks(sq);
}

inline Bitboard kingAttacks(Square sq) {
    return MagicBitboards::kingAttacks(sq);
}

inline Bitboard pawnAttacks(Color c, Square sq) {
    return MagicBitboards::pawnAttacks(c, sq);
}

inline Bitboard lineBetween(Square sq1, Square sq2) {
    return MagicBitboards::lineBetween(sq1, sq2);
}

inline Bitboard lineThrough(Square sq1, Square sq2) {
    return MagicBitboards::lineThrough(sq1, sq2);
}

} // namespace Bitboard

#endif // MAGIC_BITBOARDS_H
