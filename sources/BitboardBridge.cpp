#include "../headers/BitboardBridge.h"
#include "../headers/Board.h"
#include "../headers/Square.h"
#include "../headers/Piece.h"
#include "../headers/King.h"
#include "../headers/Queen.h"
#include "../headers/Rook.h"
#include "../headers/Bishop.h"
#include "../headers/Knight.h"
#include "../headers/Pawn.h"
#include "../headers/bitboard/BitboardEngine.h"
#include <algorithm>

BitboardBridge& BitboardBridge::instance() {
    static BitboardBridge instance;
    return instance;
}

void BitboardBridge::initialize() {
    if (m_initialized) return;

    Bitboard::BitboardEngine::instance().initialize();
    m_initialized = true;
}

char BitboardBridge::pieceToChar(Piece* piece) {
    if (!piece) return ' ';

    if (dynamic_cast<King*>(piece)) return 'K';
    if (dynamic_cast<Queen*>(piece)) return 'Q';
    if (dynamic_cast<Rook*>(piece)) return 'R';
    if (dynamic_cast<Bishop*>(piece)) return 'B';
    if (dynamic_cast<Knight*>(piece)) return 'N';
    if (dynamic_cast<Pawn*>(piece)) return 'P';

    return ' ';
}

bool BitboardBridge::isWhitePiece(Piece* piece) {
    if (!piece) return false;
    // OOP layer: WHITE=1, BLACK=0
    return piece->getColor() == ChessColor::WHITE;
}

void BitboardBridge::refreshFromBoard(Board* board, ChessColor turnColor) {
    if (!m_initialized) {
        initialize();
    }

    auto& engine = Bitboard::BitboardEngine::instance();

    // Callback to get piece at each square
    auto getPieceAt = [board](const std::string& coord) -> std::pair<char, bool> {
        Square* sq = board->get_square_by_coordinate(coord);
        if (!sq) return {' ', false};

        Piece* piece = sq->getPiece();
        if (!piece) return {' ', false};

        return {pieceToChar(piece), isWhitePiece(piece)};
    };

    // Determine castling rights by checking if kings and rooks have moved
    King* whiteKing = board->getKing(ChessColor::WHITE);
    King* blackKing = board->getKing(ChessColor::BLACK);
    std::vector<Rook*> whiteRooks = board->getRooks(ChessColor::WHITE);
    std::vector<Rook*> blackRooks = board->getRooks(ChessColor::BLACK);

    bool whiteCanCastleKingside = false;
    bool whiteCanCastleQueenside = false;
    bool blackCanCastleKingside = false;
    bool blackCanCastleQueenside = false;

    if (whiteKing && !whiteKing->hasMoved()) {
        for (Rook* rook : whiteRooks) {
            if (!rook->hasMoved()) {
                std::string rookCoord = rook->getSquare()->getCoordinate();
                if (rookCoord == "h1") whiteCanCastleKingside = true;
                else if (rookCoord == "a1") whiteCanCastleQueenside = true;
            }
        }
    }

    if (blackKing && !blackKing->hasMoved()) {
        for (Rook* rook : blackRooks) {
            if (!rook->hasMoved()) {
                std::string rookCoord = rook->getSquare()->getCoordinate();
                if (rookCoord == "h8") blackCanCastleKingside = true;
                else if (rookCoord == "a8") blackCanCastleQueenside = true;
            }
        }
    }

    // Check for en passant square
    // This requires looking at the last move in MoveLogger
    std::string enPassantSquare = "";
    // En passant detection would require access to MoveLogger
    // For now, we'll rely on the OOP layer's en passant handling or pass empty

    // OOP layer: WHITE=1, BLACK=0 -> convert to engine format
    bool whiteToMove = (turnColor == ChessColor::WHITE);

    engine.syncFromBoard(
        getPieceAt,
        whiteToMove,
        whiteCanCastleKingside,
        whiteCanCastleQueenside,
        blackCanCastleKingside,
        blackCanCastleQueenside,
        enPassantSquare
    );

    m_needsSync = false;
    m_lastSyncBoard = board;
    m_lastSyncTurn = turnColor;
}

void BitboardBridge::syncFromBoard(Board* board, ChessColor turnColor) {
    refreshFromBoard(board, turnColor);
}

std::vector<Square*> BitboardBridge::getLegalMoveSquares(Piece* piece, Board* board) {
    std::vector<Square*> result;

    if (!piece || !board) return result;

    // Ensure engine is synced
    // We need to know the current turn - get it from the piece color (assuming it's that piece's turn)
    ChessColor turnColor = piece->getColor();

    // Only resync if needed or board changed
    if (m_needsSync || m_lastSyncBoard != board || m_lastSyncTurn != turnColor) {
        refreshFromBoard(board, turnColor);
    }

    auto& engine = Bitboard::BitboardEngine::instance();

    std::string fromCoord = piece->getSquare()->getCoordinate();
    std::vector<std::string> moveSquares = engine.getLegalMoveSquares(fromCoord);

    for (const std::string& coord : moveSquares) {
        Square* sq = board->get_square_by_coordinate(coord);
        if (sq) {
            result.push_back(sq);
        }
    }

    return result;
}

bool BitboardBridge::isLegalMove(Piece* piece, Square* target, Board* board) {
    if (!piece || !target || !board) return false;

    ChessColor turnColor = piece->getColor();

    if (m_needsSync || m_lastSyncBoard != board || m_lastSyncTurn != turnColor) {
        refreshFromBoard(board, turnColor);
    }

    auto& engine = Bitboard::BitboardEngine::instance();

    std::string from = piece->getSquare()->getCoordinate();
    std::string to = target->getCoordinate();

    return engine.isLegalMove(from, to);
}

bool BitboardBridge::isInCheck(ChessColor color) const {
    auto& engine = Bitboard::BitboardEngine::instance();

    // OOP: WHITE=1, BLACK=0, Engine: WHITE=0, BLACK=1
    bool whiteToMove = engine.isWhiteToMove();
    bool checkingWhite = (color == ChessColor::WHITE);

    // If it's the color's turn and they're in check
    if (whiteToMove == checkingWhite) {
        return engine.isCheck();
    }

    // Otherwise we'd need to check the other side
    // This is a simplification - full implementation would require position query
    return false;
}

bool BitboardBridge::isCheckmate() const {
    return Bitboard::BitboardEngine::instance().isCheckmate();
}

bool BitboardBridge::isStalemate() const {
    return Bitboard::BitboardEngine::instance().isStalemate();
}

bool BitboardBridge::hasLegalMoves(ChessColor color) const {
    auto& engine = Bitboard::BitboardEngine::instance();
    bool whiteToMove = engine.isWhiteToMove();
    bool checkingWhite = (color == ChessColor::WHITE);

    if (whiteToMove == checkingWhite) {
        return engine.hasLegalMoves();
    }

    // If it's not this color's turn, we can't directly check
    return true;  // Assume they have moves
}

bool BitboardBridge::isDraw() const {
    return Bitboard::BitboardEngine::instance().isDraw();
}

bool BitboardBridge::isSquareAttacked(Square* square, ChessColor byColor) const {
    if (!square) return false;
    return isSquareAttacked(square->getCoordinate(), byColor);
}

bool BitboardBridge::isSquareAttacked(const std::string& coordinate, ChessColor byColor) const {
    auto& engine = Bitboard::BitboardEngine::instance();
    bool byWhite = (byColor == ChessColor::WHITE);
    return engine.isSquareAttacked(coordinate, byWhite);
}

int BitboardBridge::getAttackCount(const std::string& coordinate, ChessColor byColor) const {
    auto& engine = Bitboard::BitboardEngine::instance();
    bool byWhite = (byColor == ChessColor::WHITE);
    return static_cast<int>(engine.getAttackers(coordinate, byWhite).size());
}

bool BitboardBridge::isCastlingMove(Piece* king, Square* target) const {
    if (!king || !target) return false;
    if (!dynamic_cast<King*>(king)) return false;

    std::string from = king->getSquare()->getCoordinate();
    std::string to = target->getCoordinate();

    return Bitboard::BitboardEngine::instance().isCastlingMove(from, to);
}

bool BitboardBridge::isEnPassantMove(Piece* pawn, Square* target) const {
    if (!pawn || !target) return false;
    if (!dynamic_cast<Pawn*>(pawn)) return false;

    std::string from = pawn->getSquare()->getCoordinate();
    std::string to = target->getCoordinate();

    return Bitboard::BitboardEngine::instance().isEnPassantMove(from, to);
}

bool BitboardBridge::isPromotionMove(Piece* pawn, Square* target) const {
    if (!pawn || !target) return false;
    if (!dynamic_cast<Pawn*>(pawn)) return false;

    std::string from = pawn->getSquare()->getCoordinate();
    std::string to = target->getCoordinate();

    return Bitboard::BitboardEngine::instance().isPromotionMove(from, to);
}

std::pair<std::string, std::string> BitboardBridge::getCastlingRookMove(
    const std::string& kingFrom, const std::string& kingTo
) const {
    auto result = Bitboard::BitboardEngine::instance().getCastlingRookMove(kingFrom, kingTo);
    if (result) {
        return *result;
    }
    return {"", ""};
}

std::string BitboardBridge::getEnPassantCaptureSquare(
    const std::string& pawnFrom, const std::string& pawnTo
) const {
    auto result = Bitboard::BitboardEngine::instance().getEnPassantCaptureSquare(pawnFrom, pawnTo);
    if (result) {
        return *result;
    }
    return "";
}

void BitboardBridge::notifyMoveMade(const std::string& from, const std::string& to, char promotionPiece) {
    Bitboard::BitboardEngine::instance().makeMove(from, to, promotionPiece);
    m_needsSync = true;  // Mark that OOP board state may have diverged
}

void BitboardBridge::printEngineBoard() const {
    Bitboard::BitboardEngine::instance().printBoard();
}
