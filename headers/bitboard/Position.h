#ifndef BITBOARD_POSITION_H
#define BITBOARD_POSITION_H

#include "BitboardTypes.h"
#include "MagicBitboards.h"
#include "Zobrist.h"
#include <string>
#include <vector>

namespace Bitboard {

// State information that needs to be preserved for unmake
struct StateInfo {
    CastlingRights castlingRights;
    Square enPassantSquare;
    int halfmoveClock;
    PieceType capturedPiece;
    uint64_t hash;
    Bitboard checkers;
    Bitboard pinned;
    Bitboard blockers;  // Pieces blocking check on our king
    StateInfo* previous;
};

class Position {
public:
    Position();

    // Setup
    void reset();  // Standard starting position
    void setFromFEN(const std::string& fen);
    std::string toFEN() const;
    void clear();  // Empty board

    // Piece access
    Bitboard pieces() const;  // All pieces
    Bitboard pieces(Color c) const;
    Bitboard pieces(PieceType pt) const;
    Bitboard pieces(Color c, PieceType pt) const;
    Bitboard pieces(PieceType pt1, PieceType pt2) const;
    Bitboard pieces(Color c, PieceType pt1, PieceType pt2) const;

    PieceType pieceOn(Square sq) const;
    Color colorOn(Square sq) const;
    bool isEmpty(Square sq) const;

    Square kingSquare(Color c) const;
    int pieceCount(Color c, PieceType pt) const;
    int pieceCount(PieceType pt) const;

    // State access
    Color sideToMove() const { return m_sideToMove; }
    CastlingRights castlingRights() const { return m_state->castlingRights; }
    Square enPassantSquare() const { return m_state->enPassantSquare; }
    int halfmoveClock() const { return m_state->halfmoveClock; }
    int fullmoveNumber() const { return m_fullmoveNumber; }
    uint64_t hash() const { return m_state->hash; }

    bool canCastle(CastlingRights cr) const;
    bool canCastle(Color c) const;

    // Attack information
    Bitboard attackersTo(Square sq) const;
    Bitboard attackersTo(Square sq, Bitboard occupied) const;
    Bitboard attackersTo(Square sq, Color by) const;
    Bitboard attackedBy(Color c) const;
    Bitboard attackedBy(Color c, PieceType pt) const;
    bool isAttacked(Square sq, Color by) const;

    Bitboard checkers() const { return m_state->checkers; }
    bool inCheck() const { return m_state->checkers != 0; }
    Bitboard pinnedPieces(Color c) const;
    Bitboard blockersForKing(Color c) const;

    // Slider blockers (pieces that block a sliding attack on the king)
    Bitboard sliderBlockers(Bitboard sliders, Square sq, Bitboard& pinners) const;

    // Move execution
    void makeMove(Move m, StateInfo& newState);
    void unmakeMove(Move m);
    void makeNullMove(StateInfo& newState);
    void unmakeNullMove();

    // Move validation
    bool isLegal(Move m) const;
    bool isPseudoLegal(Move m) const;
    bool givesCheck(Move m) const;
    bool isCapture(Move m) const;

    // Game state
    bool isCheckmate() const;
    bool isStalemate() const;
    bool isDraw() const;
    bool hasRepeated() const;
    bool hasInsufficientMaterial() const;

    // Debug
    void print() const;
    bool isValid() const;

    // Direct piece manipulation (for bridge synchronization)
    void putPiece(PieceType pt, Color c, Square sq);
    void removePiece(Square sq);
    void setSideToMove(Color c) { m_sideToMove = c; }
    void setCastlingRights(CastlingRights cr);
    void setEnPassantSquare(Square sq);
    void setHalfmoveClock(int hmc) { m_state->halfmoveClock = hmc; }
    void setFullmoveNumber(int fmn) { m_fullmoveNumber = fmn; }
    void updateState();  // Recompute hash, checkers, pinned pieces

private:
    // Internal piece manipulation
    void movePiece(Square from, Square to);

    // State update helpers
    void updateCheckers();
    void updatePinned();
    void computeHash();

    // Bitboards
    std::array<Bitboard, PIECE_TYPE_COUNT> m_byType;
    std::array<Bitboard, COLOR_COUNT> m_byColor;

    // Mailbox for fast lookup
    std::array<PieceType, SQUARE_COUNT> m_board;
    std::array<Color, SQUARE_COUNT> m_boardColor;

    // King positions cached for fast access
    std::array<Square, COLOR_COUNT> m_kingSquare;

    // Game state
    Color m_sideToMove;
    int m_fullmoveNumber;
    StateInfo* m_state;
    StateInfo m_rootState;

    // Position history for repetition detection
    std::vector<uint64_t> m_history;
};

// Starting FEN
constexpr const char* START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

} // namespace Bitboard

#endif // BITBOARD_POSITION_H
