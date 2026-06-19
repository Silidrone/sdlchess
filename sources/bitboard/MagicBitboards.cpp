#include "../../headers/bitboard/MagicBitboards.h"
#include <cstring>
#include <vector>

namespace Bitboard {

// Static member definitions
std::array<Bitboard, 64> MagicBitboards::s_knightAttacks;
std::array<Bitboard, 64> MagicBitboards::s_kingAttacks;
std::array<std::array<Bitboard, 64>, 2> MagicBitboards::s_pawnAttacks;
std::array<Bitboard, 64> MagicBitboards::s_pseudoBishopAttacks;
std::array<Bitboard, 64> MagicBitboards::s_pseudoRookAttacks;
std::array<Magic, 64> MagicBitboards::s_bishopMagics;
std::array<Magic, 64> MagicBitboards::s_rookMagics;
std::array<Bitboard, 0x1480> MagicBitboards::s_bishopTable;
std::array<Bitboard, 0x19000> MagicBitboards::s_rookTable;
std::array<std::array<Bitboard, 64>, 64> MagicBitboards::s_lineBetween;
std::array<std::array<Bitboard, 64>, 64> MagicBitboards::s_lineThrough;
bool MagicBitboards::s_initialized = false;
uint64_t MagicBitboards::s_rngState = 0x12345678DEADBEEFULL;

// Number of relevant bits for each square (bishop)
constexpr int BishopBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

// Number of relevant bits for each square (rook)
constexpr int RookBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

void MagicBitboards::initialize() {
    if (s_initialized) return;

    initKnightAttacks();
    initKingAttacks();
    initPawnAttacks();
    initMagics(true);   // Bishop
    initMagics(false);  // Rook
    initLines();

    s_initialized = true;
}

void MagicBitboards::initKnightAttacks() {
    for (Square sq = 0; sq < 64; ++sq) {
        Bitboard attacks = 0;
        Bitboard bb = squareBB(sq);

        // All 8 knight moves
        attacks |= (bb << 17) & ~FILE_A;  // Up 2, right 1
        attacks |= (bb << 15) & ~FILE_H;  // Up 2, left 1
        attacks |= (bb << 10) & ~(FILE_A | FILE_B);  // Up 1, right 2
        attacks |= (bb << 6)  & ~(FILE_G | FILE_H);  // Up 1, left 2
        attacks |= (bb >> 17) & ~FILE_H;  // Down 2, left 1
        attacks |= (bb >> 15) & ~FILE_A;  // Down 2, right 1
        attacks |= (bb >> 10) & ~(FILE_G | FILE_H);  // Down 1, left 2
        attacks |= (bb >> 6)  & ~(FILE_A | FILE_B);  // Down 1, right 2

        s_knightAttacks[sq] = attacks;
    }
}

void MagicBitboards::initKingAttacks() {
    for (Square sq = 0; sq < 64; ++sq) {
        Bitboard attacks = 0;
        Bitboard bb = squareBB(sq);

        // All 8 king moves
        attacks |= shiftNorth(bb);
        attacks |= shiftSouth(bb);
        attacks |= shiftEast(bb);
        attacks |= shiftWest(bb);
        attacks |= shiftNorthEast(bb);
        attacks |= shiftNorthWest(bb);
        attacks |= shiftSouthEast(bb);
        attacks |= shiftSouthWest(bb);

        s_kingAttacks[sq] = attacks;
    }
}

void MagicBitboards::initPawnAttacks() {
    for (Square sq = 0; sq < 64; ++sq) {
        Bitboard bb = squareBB(sq);

        // White pawn attacks (north-east and north-west)
        s_pawnAttacks[WHITE][sq] = shiftNorthEast(bb) | shiftNorthWest(bb);

        // Black pawn attacks (south-east and south-west)
        s_pawnAttacks[BLACK][sq] = shiftSouthEast(bb) | shiftSouthWest(bb);
    }
}

Bitboard MagicBitboards::computeSlidingAttacks(Square sq, Bitboard occupied, bool isBishop) {
    Bitboard attacks = 0;
    int rank = rankOf(sq);
    int file = fileOf(sq);

    if (isBishop) {
        // Bishop: diagonal directions
        int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (auto& dir : directions) {
            int r = rank + dir[0];
            int f = file + dir[1];
            while (r >= 0 && r <= 7 && f >= 0 && f <= 7) {
                Square target = makeSquare(f, r);
                attacks |= squareBB(target);
                if (occupied & squareBB(target)) break;
                r += dir[0];
                f += dir[1];
            }
        }
    } else {
        // Rook: orthogonal directions
        int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        for (auto& dir : directions) {
            int r = rank + dir[0];
            int f = file + dir[1];
            while (r >= 0 && r <= 7 && f >= 0 && f <= 7) {
                Square target = makeSquare(f, r);
                attacks |= squareBB(target);
                if (occupied & squareBB(target)) break;
                r += dir[0];
                f += dir[1];
            }
        }
    }

    return attacks;
}

Bitboard MagicBitboards::computeRelevantOccupancy(Square sq, bool isBishop) {
    Bitboard attacks = 0;
    int rank = rankOf(sq);
    int file = fileOf(sq);

    if (isBishop) {
        // Bishop: diagonal directions, excluding edges
        int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (auto& dir : directions) {
            int r = rank + dir[0];
            int f = file + dir[1];
            // Stop one square before the edge
            while (r > 0 && r < 7 && f > 0 && f < 7) {
                attacks |= squareBB(makeSquare(f, r));
                r += dir[0];
                f += dir[1];
            }
        }
    } else {
        // Rook: orthogonal directions, excluding edges
        // North
        for (int r = rank + 1; r < 7; ++r)
            attacks |= squareBB(makeSquare(file, r));
        // South
        for (int r = rank - 1; r > 0; --r)
            attacks |= squareBB(makeSquare(file, r));
        // East
        for (int f = file + 1; f < 7; ++f)
            attacks |= squareBB(makeSquare(f, rank));
        // West
        for (int f = file - 1; f > 0; --f)
            attacks |= squareBB(makeSquare(f, rank));
    }

    return attacks;
}

uint64_t MagicBitboards::random64() {
    // xorshift64 PRNG
    s_rngState ^= s_rngState >> 12;
    s_rngState ^= s_rngState << 25;
    s_rngState ^= s_rngState >> 27;
    return s_rngState * 0x2545F4914F6CDD1DULL;
}

uint64_t MagicBitboards::random64Sparse() {
    // Generate a sparse random number (fewer bits set)
    // This helps find magic numbers faster
    return random64() & random64() & random64();
}

Bitboard MagicBitboards::findMagic(Square sq, bool isBishop) {
    Bitboard mask = computeRelevantOccupancy(sq, isBishop);
    int bits = popcount(mask);
    int tableSize = 1 << bits;

    // Generate all possible occupancy configurations
    std::vector<Bitboard> occupancies(tableSize);
    std::vector<Bitboard> attacks(tableSize);

    for (int i = 0; i < tableSize; ++i) {
        // Map index to occupancy using PEXT-like enumeration
        Bitboard occ = 0;
        Bitboard m = mask;
        for (int j = 0; j < bits; ++j) {
            Square s = popLsb(m);
            if (i & (1 << j)) {
                occ |= squareBB(s);
            }
        }
        occupancies[i] = occ;
        attacks[i] = computeSlidingAttacks(sq, occ, isBishop);
    }

    // Try random magic numbers until we find one that works
    std::vector<Bitboard> used(tableSize);

    for (int attempt = 0; attempt < 100000000; ++attempt) {
        Bitboard magic = random64Sparse();

        // Quick check: skip if magic has too few bits in high positions
        if (popcount((mask * magic) & 0xFF00000000000000ULL) < 6) continue;

        // Clear used table
        std::fill(used.begin(), used.end(), 0);

        bool failed = false;
        for (int i = 0; i < tableSize && !failed; ++i) {
            unsigned idx = static_cast<unsigned>((occupancies[i] * magic) >> (64 - bits));

            if (used[idx] == 0) {
                used[idx] = attacks[i];
            } else if (used[idx] != attacks[i]) {
                failed = true;  // Collision with different attack set
            }
        }

        if (!failed) {
            return magic;
        }
    }

    // Should not reach here with reasonable parameters
    return 0;
}

void MagicBitboards::initMagics(bool isBishop) {
    Bitboard* table = isBishop ? s_bishopTable.data() : s_rookTable.data();
    std::array<Magic, 64>& magics = isBishop ? s_bishopMagics : s_rookMagics;
    const int* bitCounts = isBishop ? BishopBits : RookBits;

    Bitboard* currentTable = table;

    for (Square sq = 0; sq < 64; ++sq) {
        Magic& m = magics[sq];

        m.mask = computeRelevantOccupancy(sq, isBishop);
        m.shift = 64 - bitCounts[sq];
        m.attacks = currentTable;

        // Find a working magic number
        m.magic = findMagic(sq, isBishop);

        // Populate the attack table
        int tableSize = 1 << bitCounts[sq];
        int bits = bitCounts[sq];

        for (int i = 0; i < tableSize; ++i) {
            // Map index to occupancy
            Bitboard occ = 0;
            Bitboard mask = m.mask;
            for (int j = 0; j < bits; ++j) {
                Square s = popLsb(mask);
                if (i & (1 << j)) {
                    occ |= squareBB(s);
                }
            }

            unsigned idx = m.index(occ);
            m.attacks[idx] = computeSlidingAttacks(sq, occ, isBishop);
        }

        currentTable += tableSize;
    }

    // Also compute pseudo attacks (on empty board)
    for (Square sq = 0; sq < 64; ++sq) {
        if (isBishop) {
            s_pseudoBishopAttacks[sq] = computeSlidingAttacks(sq, 0, true);
        } else {
            s_pseudoRookAttacks[sq] = computeSlidingAttacks(sq, 0, false);
        }
    }
}

void MagicBitboards::initLines() {
    for (Square sq1 = 0; sq1 < 64; ++sq1) {
        for (Square sq2 = 0; sq2 < 64; ++sq2) {
            s_lineBetween[sq1][sq2] = 0;
            s_lineThrough[sq1][sq2] = 0;

            if (sq1 == sq2) continue;

            // Check if squares are on same diagonal or anti-diagonal
            Bitboard bishopAttacks1 = s_pseudoBishopAttacks[sq1];
            if (bishopAttacks1 & squareBB(sq2)) {
                // Squares are on same diagonal
                s_lineThrough[sq1][sq2] = (bishopAttacks1 & s_pseudoBishopAttacks[sq2]) | squareBB(sq1) | squareBB(sq2);
                s_lineBetween[sq1][sq2] = bishopAttacks(sq1, squareBB(sq2)) & bishopAttacks(sq2, squareBB(sq1));
            }

            // Check if squares are on same rank or file
            Bitboard rookAttacks1 = s_pseudoRookAttacks[sq1];
            if (rookAttacks1 & squareBB(sq2)) {
                // Squares are on same line
                s_lineThrough[sq1][sq2] = (rookAttacks1 & s_pseudoRookAttacks[sq2]) | squareBB(sq1) | squareBB(sq2);
                s_lineBetween[sq1][sq2] = rookAttacks(sq1, squareBB(sq2)) & rookAttacks(sq2, squareBB(sq1));
            }
        }
    }
}

// Public lookup functions

Bitboard MagicBitboards::bishopAttacks(Square sq, Bitboard occupied) {
    const Magic& m = s_bishopMagics[sq];
    return m.attacks[m.index(occupied)];
}

Bitboard MagicBitboards::rookAttacks(Square sq, Bitboard occupied) {
    const Magic& m = s_rookMagics[sq];
    return m.attacks[m.index(occupied)];
}

Bitboard MagicBitboards::queenAttacks(Square sq, Bitboard occupied) {
    return bishopAttacks(sq, occupied) | rookAttacks(sq, occupied);
}

Bitboard MagicBitboards::knightAttacks(Square sq) {
    return s_knightAttacks[sq];
}

Bitboard MagicBitboards::kingAttacks(Square sq) {
    return s_kingAttacks[sq];
}

Bitboard MagicBitboards::pawnAttacks(Color c, Square sq) {
    return s_pawnAttacks[c][sq];
}

Bitboard MagicBitboards::pawnAttacks(Color c, Bitboard pawns) {
    if (c == WHITE) {
        return shiftNorthEast(pawns) | shiftNorthWest(pawns);
    } else {
        return shiftSouthEast(pawns) | shiftSouthWest(pawns);
    }
}

Bitboard MagicBitboards::lineBetween(Square sq1, Square sq2) {
    return s_lineBetween[sq1][sq2];
}

Bitboard MagicBitboards::lineThrough(Square sq1, Square sq2) {
    return s_lineThrough[sq1][sq2];
}

Bitboard MagicBitboards::pseudoBishopAttacks(Square sq) {
    return s_pseudoBishopAttacks[sq];
}

Bitboard MagicBitboards::pseudoRookAttacks(Square sq) {
    return s_pseudoRookAttacks[sq];
}

} // namespace Bitboard
