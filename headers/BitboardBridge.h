#ifndef BITBOARD_BRIDGE_H
#define BITBOARD_BRIDGE_H

#include <vector>
#include <string>
#include "ChessColored.h"

// Forward declarations
class Square;
class Piece;
class Board;
class King;
class Rook;

// Bridge between OOP layer and BitboardEngine
class BitboardBridge {
public:
    // Get singleton instance
    static BitboardBridge& instance();

    // Initialize the bitboard engine (call once at startup)
    void initialize();
    bool isInitialized() const { return m_initialized; }

    // Synchronize bitboard engine state from OOP Board
    void syncFromBoard(Board* board, ChessColor turnColor);

    // Get legal move squares for a piece (replaces attacked_squares + moveable_squares)
    std::vector<Square*> getLegalMoveSquares(Piece* piece, Board* board);

    // Check if a move is legal
    bool isLegalMove(Piece* piece, Square* target, Board* board);

    // Position queries
    bool isInCheck(ChessColor color) const;
    bool isCheckmate() const;
    bool isStalemate() const;
    bool hasLegalMoves(ChessColor color) const;
    bool isDraw() const;

    // Attack queries
    bool isSquareAttacked(Square* square, ChessColor byColor) const;
    bool isSquareAttacked(const std::string& coordinate, ChessColor byColor) const;
    int getAttackCount(const std::string& coordinate, ChessColor byColor) const;

    // Special move detection
    bool isCastlingMove(Piece* king, Square* target) const;
    bool isEnPassantMove(Piece* pawn, Square* target) const;
    bool isPromotionMove(Piece* pawn, Square* target) const;

    // Get castling rook info: returns pair<rookFromCoord, rookToCoord>
    std::pair<std::string, std::string> getCastlingRookMove(
        const std::string& kingFrom, const std::string& kingTo) const;

    // Get en passant captured pawn square
    std::string getEnPassantCaptureSquare(
        const std::string& pawnFrom, const std::string& pawnTo) const;

    // After move is made in OOP layer, update engine state
    void notifyMoveMade(const std::string& from, const std::string& to,
                        char promotionPiece = '\0');

    // Mark that sync is needed
    void markNeedsSync() { m_needsSync = true; }

    // Debug
    void printEngineBoard() const;

private:
    BitboardBridge() : m_initialized(false), m_needsSync(true), m_lastSyncBoard(nullptr) {}
    BitboardBridge(const BitboardBridge&) = delete;
    BitboardBridge& operator=(const BitboardBridge&) = delete;

    // Helper functions
    static char pieceToChar(Piece* piece);
    static bool isWhitePiece(Piece* piece);

    // Sync helper
    void refreshFromBoard(Board* board, ChessColor turnColor);

    bool m_initialized;
    bool m_needsSync;
    Board* m_lastSyncBoard;
    ChessColor m_lastSyncTurn;
};

#endif // BITBOARD_BRIDGE_H
