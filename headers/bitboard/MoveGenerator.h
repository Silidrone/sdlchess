#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "Position.h"
#include <vector>

namespace Bitboard {

// Fixed-size move list to avoid heap allocation
class MoveList {
public:
    static constexpr int MAX_MOVES = 256;

    MoveList() : m_size(0) {}

    void push(Move m) {
        if (m_size < MAX_MOVES) {
            m_moves[m_size++] = m;
        }
    }

    Move operator[](int i) const { return m_moves[i]; }
    Move& operator[](int i) { return m_moves[i]; }

    int size() const { return m_size; }
    bool empty() const { return m_size == 0; }
    void clear() { m_size = 0; }

    Move* begin() { return m_moves; }
    Move* end() { return m_moves + m_size; }
    const Move* begin() const { return m_moves; }
    const Move* end() const { return m_moves + m_size; }

    Move back() const { return m_moves[m_size - 1]; }
    void pop_back() { if (m_size > 0) --m_size; }

private:
    Move m_moves[MAX_MOVES];
    int m_size;
};

class MoveGenerator {
public:
    // Generate all legal moves
    static void generateLegalMoves(const Position& pos, MoveList& moves);

    // Generate all pseudo-legal moves (may leave king in check)
    static void generatePseudoLegalMoves(const Position& pos, MoveList& moves);

    // Generate only captures (for quiescence search)
    static void generateCaptures(const Position& pos, MoveList& moves);

    // Generate only quiet moves (non-captures)
    static void generateQuiets(const Position& pos, MoveList& moves);

    // Generate check evasions when in check
    static void generateEvasions(const Position& pos, MoveList& moves);

    // Check if position has any legal moves
    static bool hasLegalMoves(const Position& pos);

    // Get legal moves for a specific piece
    static void getLegalMovesForPiece(const Position& pos, Square sq, MoveList& moves);

    // Perft - count nodes at given depth
    static uint64_t perft(Position& pos, int depth);

private:
    // Generate moves for specific piece types
    static void generatePawnMoves(const Position& pos, MoveList& moves, Bitboard targets, bool capturesOnly);
    static void generateKnightMoves(const Position& pos, MoveList& moves, Bitboard targets);
    static void generateBishopMoves(const Position& pos, MoveList& moves, Bitboard targets);
    static void generateRookMoves(const Position& pos, MoveList& moves, Bitboard targets);
    static void generateQueenMoves(const Position& pos, MoveList& moves, Bitboard targets);
    static void generateKingMoves(const Position& pos, MoveList& moves, Bitboard targets);
    static void generateCastlingMoves(const Position& pos, MoveList& moves);

    // Helpers for pawn moves
    static void addPawnMoves(MoveList& moves, Bitboard pawns, int delta, MoveType type = NORMAL);
    static void addPromotions(MoveList& moves, Square from, Square to);
    static void addPromotionCaptures(MoveList& moves, Square from, Square to);
};

} // namespace Bitboard

#endif // MOVE_GENERATOR_H
