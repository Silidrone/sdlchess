#ifndef BITBOARD_ENGINE_H
#define BITBOARD_ENGINE_H

#include "Position.h"
#include "MoveGenerator.h"
#include <string>
#include <vector>
#include <optional>
#include <functional>

namespace Bitboard {

// High-level move representation for OOP layer
struct EngineMove {
    std::string from;
    std::string to;
    MoveType type;
    PieceType promotionPiece;
    bool isCapture;
    bool givesCheck;

    // Convert to internal Move format
    Move toInternal() const;

    // Create from internal Move and position
    static EngineMove fromInternal(Move m, const Position& pos);
};

class BitboardEngine {
public:
    // Get singleton instance
    static BitboardEngine& instance();

    // Initialize engine (call once at startup)
    void initialize();
    bool isInitialized() const { return m_initialized; }

    // Position setup
    void reset();  // Start position
    void setPosition(const std::string& fen);
    std::string getFEN() const;
    void clear();

    // Synchronize from external board state
    // Callback: getPieceAt(coordinate) returns {piece_char, is_white}
    // piece_char is one of 'P','N','B','R','Q','K' or ' ' for empty
    void syncFromBoard(
        const std::function<std::pair<char, bool>(const std::string&)>& getPieceAt,
        bool whiteToMove,
        bool whiteCanCastleKingside,
        bool whiteCanCastleQueenside,
        bool blackCanCastleKingside,
        bool blackCanCastleQueenside,
        const std::string& enPassantSquare = ""
    );

    // Move generation
    std::vector<EngineMove> getLegalMoves() const;
    std::vector<EngineMove> getLegalMoves(const std::string& fromSquare) const;
    std::vector<std::string> getLegalMoveSquares(const std::string& fromSquare) const;
    bool hasLegalMoves() const;

    // Move validation
    bool isLegalMove(const std::string& from, const std::string& to,
                     char promotionPiece = '\0') const;

    // Move execution
    bool makeMove(const std::string& from, const std::string& to,
                  char promotionPiece = '\0');
    void unmakeMove();

    // Position queries
    bool isCheck() const;
    bool isCheckmate() const;
    bool isStalemate() const;
    bool isDraw() const;
    bool isGameOver() const;

    std::string getKingSquare(bool white) const;

    // Attack queries
    bool isSquareAttacked(const std::string& square, bool byWhite) const;
    std::vector<std::string> getAttackers(const std::string& square, bool byWhite) const;

    // Piece queries
    char getPieceAt(const std::string& square) const;
    bool hasPieceAt(const std::string& square) const;
    bool isWhitePieceAt(const std::string& square) const;

    // Side to move
    bool isWhiteToMove() const;

    // Special move detection
    bool isCastlingMove(const std::string& from, const std::string& to) const;
    bool isEnPassantMove(const std::string& from, const std::string& to) const;
    bool isPromotionMove(const std::string& from, const std::string& to) const;

    // Get info about special moves
    std::optional<std::pair<std::string, std::string>> getCastlingRookMove(
        const std::string& kingFrom, const std::string& kingTo) const;
    std::optional<std::string> getEnPassantCaptureSquare(
        const std::string& from, const std::string& to) const;

    // Perft testing
    uint64_t perft(int depth);

    // Debug
    void printBoard() const;

private:
    BitboardEngine() : m_initialized(false) {}
    BitboardEngine(const BitboardEngine&) = delete;
    BitboardEngine& operator=(const BitboardEngine&) = delete;

    Position m_position;
    std::vector<StateInfo> m_stateHistory;
    bool m_initialized;

    // Helper to find internal move
    Move findMove(const std::string& from, const std::string& to, char promo) const;
};

} // namespace Bitboard

#endif // BITBOARD_ENGINE_H
