#ifndef BITBOARD_UTILS_H
#define BITBOARD_UTILS_H

#include "BitboardTypes.h"

namespace Bitboard {

// Population count (number of set bits)
inline int popcount(Bitboard bb) {
    return __builtin_popcountll(bb);
}

// Bit scan forward - find index of least significant bit
// Undefined behavior if bb == 0
inline Square lsb(Bitboard bb) {
    return static_cast<Square>(__builtin_ctzll(bb));
}

// Bit scan reverse - find index of most significant bit
// Undefined behavior if bb == 0
inline Square msb(Bitboard bb) {
    return static_cast<Square>(63 - __builtin_clzll(bb));
}

// Pop least significant bit and return its index
inline Square popLsb(Bitboard& bb) {
    Square sq = lsb(bb);
    bb &= bb - 1;  // Clear the LSB
    return sq;
}

// Check if exactly one bit is set
inline bool onlyOne(Bitboard bb) {
    return bb && !(bb & (bb - 1));
}

// Check if more than one bit is set
inline bool moreThanOne(Bitboard bb) {
    return bb & (bb - 1);
}

// Shift operations with edge wrapping protection

inline Bitboard shiftNorth(Bitboard bb) {
    return bb << 8;
}

inline Bitboard shiftSouth(Bitboard bb) {
    return bb >> 8;
}

inline Bitboard shiftEast(Bitboard bb) {
    return (bb & ~FILE_H) << 1;
}

inline Bitboard shiftWest(Bitboard bb) {
    return (bb & ~FILE_A) >> 1;
}

inline Bitboard shiftNorthEast(Bitboard bb) {
    return (bb & ~FILE_H) << 9;
}

inline Bitboard shiftNorthWest(Bitboard bb) {
    return (bb & ~FILE_A) << 7;
}

inline Bitboard shiftSouthEast(Bitboard bb) {
    return (bb & ~FILE_H) >> 7;
}

inline Bitboard shiftSouthWest(Bitboard bb) {
    return (bb & ~FILE_A) >> 9;
}

// Double pawn pushes
inline Bitboard shiftNorthNorth(Bitboard bb) {
    return bb << 16;
}

inline Bitboard shiftSouthSouth(Bitboard bb) {
    return bb >> 16;
}

// Generic shift in direction (positive = north, negative = south)
inline Bitboard shift(Bitboard bb, int direction) {
    switch (direction) {
        case NORTH:      return shiftNorth(bb);
        case SOUTH:      return shiftSouth(bb);
        case EAST:       return shiftEast(bb);
        case WEST:       return shiftWest(bb);
        case NORTH_EAST: return shiftNorthEast(bb);
        case NORTH_WEST: return shiftNorthWest(bb);
        case SOUTH_EAST: return shiftSouthEast(bb);
        case SOUTH_WEST: return shiftSouthWest(bb);
        default:         return bb;
    }
}

// Pawn direction based on color
inline Bitboard pawnPush(Bitboard bb, Color c) {
    return c == WHITE ? shiftNorth(bb) : shiftSouth(bb);
}

inline Bitboard pawnDoublePush(Bitboard bb, Color c) {
    return c == WHITE ? shiftNorthNorth(bb) : shiftSouthSouth(bb);
}

inline Bitboard pawnAttacksLeft(Bitboard bb, Color c) {
    return c == WHITE ? shiftNorthWest(bb) : shiftSouthEast(bb);
}

inline Bitboard pawnAttacksRight(Bitboard bb, Color c) {
    return c == WHITE ? shiftNorthEast(bb) : shiftSouthWest(bb);
}

// Get rank mask for pawn start rank
inline Bitboard pawnStartRank(Color c) {
    return c == WHITE ? RANK_2 : RANK_7;
}

// Get rank mask for pawn promotion rank
inline Bitboard promotionRank(Color c) {
    return c == WHITE ? RANK_8 : RANK_1;
}

// Get rank mask for en passant capture rank
inline Bitboard enPassantRank(Color c) {
    return c == WHITE ? RANK_5 : RANK_4;
}

// Fill algorithms for sliding pieces (used in magic generation)

// Fill north from occupied squares
inline Bitboard fillNorth(Bitboard bb, Bitboard empty) {
    bb |= empty & (bb << 8);
    empty &= (empty << 8);
    bb |= empty & (bb << 16);
    empty &= (empty << 16);
    bb |= empty & (bb << 32);
    return bb;
}

// Fill south from occupied squares
inline Bitboard fillSouth(Bitboard bb, Bitboard empty) {
    bb |= empty & (bb >> 8);
    empty &= (empty >> 8);
    bb |= empty & (bb >> 16);
    empty &= (empty >> 16);
    bb |= empty & (bb >> 32);
    return bb;
}

// Occluded fill algorithms for ray attacks
inline Bitboard occludedFillNorth(Bitboard gen, Bitboard pro) {
    gen |= pro & (gen << 8);
    pro &= (pro << 8);
    gen |= pro & (gen << 16);
    pro &= (pro << 16);
    gen |= pro & (gen << 32);
    return gen;
}

inline Bitboard occludedFillSouth(Bitboard gen, Bitboard pro) {
    gen |= pro & (gen >> 8);
    pro &= (pro >> 8);
    gen |= pro & (gen >> 16);
    pro &= (pro >> 16);
    gen |= pro & (gen >> 32);
    return gen;
}

inline Bitboard occludedFillEast(Bitboard gen, Bitboard pro) {
    pro &= ~FILE_A;
    gen |= pro & (gen << 1);
    pro &= (pro << 1);
    gen |= pro & (gen << 2);
    pro &= (pro << 2);
    gen |= pro & (gen << 4);
    return gen;
}

inline Bitboard occludedFillWest(Bitboard gen, Bitboard pro) {
    pro &= ~FILE_H;
    gen |= pro & (gen >> 1);
    pro &= (pro >> 1);
    gen |= pro & (gen >> 2);
    pro &= (pro >> 2);
    gen |= pro & (gen >> 4);
    return gen;
}

inline Bitboard occludedFillNorthEast(Bitboard gen, Bitboard pro) {
    pro &= ~FILE_A;
    gen |= pro & (gen << 9);
    pro &= (pro << 9);
    gen |= pro & (gen << 18);
    pro &= (pro << 18);
    gen |= pro & (gen << 36);
    return gen;
}

inline Bitboard occludedFillNorthWest(Bitboard gen, Bitboard pro) {
    pro &= ~FILE_H;
    gen |= pro & (gen << 7);
    pro &= (pro << 7);
    gen |= pro & (gen << 14);
    pro &= (pro << 14);
    gen |= pro & (gen << 28);
    return gen;
}

inline Bitboard occludedFillSouthEast(Bitboard gen, Bitboard pro) {
    pro &= ~FILE_A;
    gen |= pro & (gen >> 7);
    pro &= (pro >> 7);
    gen |= pro & (gen >> 14);
    pro &= (pro >> 14);
    gen |= pro & (gen >> 28);
    return gen;
}

inline Bitboard occludedFillSouthWest(Bitboard gen, Bitboard pro) {
    pro &= ~FILE_H;
    gen |= pro & (gen >> 9);
    pro &= (pro >> 9);
    gen |= pro & (gen >> 18);
    pro &= (pro >> 18);
    gen |= pro & (gen >> 36);
    return gen;
}

// Iterator for iterating over set bits in a bitboard
class BitboardIterator {
public:
    explicit BitboardIterator(Bitboard bb) : m_bb(bb) {}

    bool hasNext() const {
        return m_bb != 0;
    }

    Square next() {
        return popLsb(m_bb);
    }

    // Range-based for loop support
    class Iterator {
    public:
        explicit Iterator(Bitboard bb) : m_bb(bb), m_sq(bb ? lsb(bb) : NO_SQUARE) {}

        Square operator*() const { return m_sq; }

        Iterator& operator++() {
            m_bb &= m_bb - 1;
            m_sq = m_bb ? lsb(m_bb) : NO_SQUARE;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return m_bb != other.m_bb;
        }

    private:
        Bitboard m_bb;
        Square m_sq;
    };

    Iterator begin() const { return Iterator(m_bb); }
    Iterator end() const { return Iterator(0); }

private:
    Bitboard m_bb;
};

// Convenience macro for iterating over squares in a bitboard
#define FOR_EACH_SQUARE(sq, bb) \
    for (Bitboard _bb = (bb); _bb; ) \
        for (Square sq = popLsb(_bb); ; _bb = 0)

// Print bitboard for debugging
inline void printBitboard(Bitboard bb) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            Square sq = makeSquare(file, rank);
            printf("%c ", (bb & squareBB(sq)) ? '1' : '.');
        }
        printf("\n");
    }
    printf("\n");
}

} // namespace Bitboard

#endif // BITBOARD_UTILS_H
