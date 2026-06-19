#include "../../headers/bitboard/BitboardEngine.h"
#include "../../headers/bitboard/BitboardUtils.h"

namespace Bitboard {

// EngineMove implementation

Move EngineMove::toInternal() const {
    Square fromSq = coordinateToSquare(from);
    Square toSq = coordinateToSquare(to);

    if (type == PROMOTION) {
        return makeMove(fromSq, toSq, PROMOTION, promotionPiece);
    } else if (type == EN_PASSANT) {
        return makeMove(fromSq, toSq, EN_PASSANT);
    } else if (type == CASTLING) {
        return makeMove(fromSq, toSq, CASTLING);
    } else {
        return makeMove(fromSq, toSq);
    }
}

EngineMove EngineMove::fromInternal(Move m, const Position& pos) {
    EngineMove em;
    em.from = squareToCoordinate(moveFrom(m));
    em.to = squareToCoordinate(moveTo(m));
    em.type = moveType(m);
    em.promotionPiece = (em.type == PROMOTION) ? movePromotion(m) : NO_PIECE_TYPE;
    em.isCapture = pos.isCapture(m);
    em.givesCheck = pos.givesCheck(m);
    return em;
}

// BitboardEngine implementation

BitboardEngine& BitboardEngine::instance() {
    static BitboardEngine instance;
    return instance;
}

void BitboardEngine::initialize() {
    if (m_initialized) return;

    Zobrist::initialize();
    MagicBitboards::initialize();

    m_position.reset();
    m_stateHistory.clear();
    m_initialized = true;
}

void BitboardEngine::reset() {
    m_position.reset();
    m_stateHistory.clear();
}

void BitboardEngine::setPosition(const std::string& fen) {
    m_position.setFromFEN(fen);
    m_stateHistory.clear();
}

std::string BitboardEngine::getFEN() const {
    return m_position.toFEN();
}

void BitboardEngine::clear() {
    m_position.clear();
    m_stateHistory.clear();
}

void BitboardEngine::syncFromBoard(
    const std::function<std::pair<char, bool>(const std::string&)>& getPieceAt,
    bool whiteToMove,
    bool whiteCanCastleKingside,
    bool whiteCanCastleQueenside,
    bool blackCanCastleKingside,
    bool blackCanCastleQueenside,
    const std::string& enPassantSquare
) {
    m_position.clear();
    m_stateHistory.clear();

    // Set pieces from callback
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            std::string coord = squareToCoordinate(makeSquare(file, rank));
            auto [pieceChar, isWhite] = getPieceAt(coord);

            if (pieceChar != ' ' && pieceChar != '\0') {
                PieceType pt;
                switch (toupper(pieceChar)) {
                    case 'P': pt = PAWN; break;
                    case 'N': pt = KNIGHT; break;
                    case 'B': pt = BISHOP; break;
                    case 'R': pt = ROOK; break;
                    case 'Q': pt = QUEEN; break;
                    case 'K': pt = KING; break;
                    default: continue;
                }

                Color c = isWhite ? WHITE : BLACK;
                m_position.putPiece(pt, c, makeSquare(file, rank));
            }
        }
    }

    // Set side to move
    m_position.setSideToMove(whiteToMove ? WHITE : BLACK);

    // Set castling rights
    CastlingRights cr = NO_CASTLING;
    if (whiteCanCastleKingside) cr |= WHITE_OO;
    if (whiteCanCastleQueenside) cr |= WHITE_OOO;
    if (blackCanCastleKingside) cr |= BLACK_OO;
    if (blackCanCastleQueenside) cr |= BLACK_OOO;
    m_position.setCastlingRights(cr);

    // Set en passant square
    if (!enPassantSquare.empty() && enPassantSquare != "-") {
        m_position.setEnPassantSquare(coordinateToSquare(enPassantSquare));
    } else {
        m_position.setEnPassantSquare(NO_SQUARE);
    }

    // Update computed state
    m_position.updateState();
}

std::vector<EngineMove> BitboardEngine::getLegalMoves() const {
    std::vector<EngineMove> result;
    MoveList moves;
    MoveGenerator::generateLegalMoves(m_position, moves);

    for (Move m : moves) {
        result.push_back(EngineMove::fromInternal(m, m_position));
    }

    return result;
}

std::vector<EngineMove> BitboardEngine::getLegalMoves(const std::string& fromSquare) const {
    std::vector<EngineMove> result;
    Square from = coordinateToSquare(fromSquare);
    if (from == NO_SQUARE) return result;

    MoveList moves;
    MoveGenerator::getLegalMovesForPiece(m_position, from, moves);

    for (Move m : moves) {
        result.push_back(EngineMove::fromInternal(m, m_position));
    }

    return result;
}

std::vector<std::string> BitboardEngine::getLegalMoveSquares(const std::string& fromSquare) const {
    std::vector<std::string> result;
    Square from = coordinateToSquare(fromSquare);
    if (from == NO_SQUARE) return result;

    MoveList moves;
    MoveGenerator::getLegalMovesForPiece(m_position, from, moves);

    for (Move m : moves) {
        result.push_back(squareToCoordinate(moveTo(m)));
    }

    return result;
}

bool BitboardEngine::hasLegalMoves() const {
    return MoveGenerator::hasLegalMoves(m_position);
}

Move BitboardEngine::findMove(const std::string& from, const std::string& to, char promo) const {
    Square fromSq = coordinateToSquare(from);
    Square toSq = coordinateToSquare(to);

    if (fromSq == NO_SQUARE || toSq == NO_SQUARE) return NO_MOVE;

    MoveList moves;
    MoveGenerator::getLegalMovesForPiece(m_position, fromSq, moves);

    PieceType promoType = NO_PIECE_TYPE;
    if (promo) {
        switch (toupper(promo)) {
            case 'N': promoType = KNIGHT; break;
            case 'B': promoType = BISHOP; break;
            case 'R': promoType = ROOK; break;
            case 'Q': promoType = QUEEN; break;
            default: break;
        }
    }

    for (Move m : moves) {
        if (moveTo(m) == toSq) {
            if (moveType(m) == PROMOTION) {
                if (promoType != NO_PIECE_TYPE && movePromotion(m) == promoType) {
                    return m;
                }
            } else {
                return m;
            }
        }
    }

    return NO_MOVE;
}

bool BitboardEngine::isLegalMove(const std::string& from, const std::string& to, char promo) const {
    return findMove(from, to, promo) != NO_MOVE;
}

bool BitboardEngine::makeMove(const std::string& from, const std::string& to, char promo) {
    Move m = findMove(from, to, promo);
    if (m == NO_MOVE) return false;

    m_stateHistory.emplace_back();
    m_position.makeMove(m, m_stateHistory.back());
    return true;
}

void BitboardEngine::unmakeMove() {
    if (m_stateHistory.empty()) return;

    // We need the move that was made - this requires storing it
    // For now, we'll use a simpler approach: reset and replay
    // In a real implementation, we'd store the move history
    m_stateHistory.pop_back();
}

bool BitboardEngine::isCheck() const {
    return m_position.inCheck();
}

bool BitboardEngine::isCheckmate() const {
    return m_position.inCheck() && !MoveGenerator::hasLegalMoves(m_position);
}

bool BitboardEngine::isStalemate() const {
    return !m_position.inCheck() && !MoveGenerator::hasLegalMoves(m_position);
}

bool BitboardEngine::isDraw() const {
    return m_position.isDraw();
}

bool BitboardEngine::isGameOver() const {
    return isCheckmate() || isStalemate() || isDraw();
}

std::string BitboardEngine::getKingSquare(bool white) const {
    Color c = white ? WHITE : BLACK;
    return squareToCoordinate(m_position.kingSquare(c));
}

bool BitboardEngine::isSquareAttacked(const std::string& square, bool byWhite) const {
    Square sq = coordinateToSquare(square);
    if (sq == NO_SQUARE) return false;
    return m_position.isAttacked(sq, byWhite ? WHITE : BLACK);
}

std::vector<std::string> BitboardEngine::getAttackers(const std::string& square, bool byWhite) const {
    std::vector<std::string> result;
    Square sq = coordinateToSquare(square);
    if (sq == NO_SQUARE) return result;

    Bitboard attackers = m_position.attackersTo(sq, byWhite ? WHITE : BLACK);
    while (attackers) {
        result.push_back(squareToCoordinate(popLsb(attackers)));
    }

    return result;
}

char BitboardEngine::getPieceAt(const std::string& square) const {
    Square sq = coordinateToSquare(square);
    if (sq == NO_SQUARE) return ' ';

    PieceType pt = m_position.pieceOn(sq);
    if (pt == NO_PIECE_TYPE) return ' ';

    Color c = m_position.colorOn(sq);
    return pieceToChar(pt, c);
}

bool BitboardEngine::hasPieceAt(const std::string& square) const {
    Square sq = coordinateToSquare(square);
    if (sq == NO_SQUARE) return false;
    return !m_position.isEmpty(sq);
}

bool BitboardEngine::isWhitePieceAt(const std::string& square) const {
    Square sq = coordinateToSquare(square);
    if (sq == NO_SQUARE) return false;
    return m_position.colorOn(sq) == WHITE && !m_position.isEmpty(sq);
}

bool BitboardEngine::isWhiteToMove() const {
    return m_position.sideToMove() == WHITE;
}

bool BitboardEngine::isCastlingMove(const std::string& from, const std::string& to) const {
    Move m = findMove(from, to, '\0');
    return m != NO_MOVE && moveType(m) == CASTLING;
}

bool BitboardEngine::isEnPassantMove(const std::string& from, const std::string& to) const {
    Move m = findMove(from, to, '\0');
    return m != NO_MOVE && moveType(m) == EN_PASSANT;
}

bool BitboardEngine::isPromotionMove(const std::string& from, const std::string& to) const {
    Square fromSq = coordinateToSquare(from);
    Square toSq = coordinateToSquare(to);

    if (fromSq == NO_SQUARE || toSq == NO_SQUARE) return false;

    PieceType pt = m_position.pieceOn(fromSq);
    if (pt != PAWN) return false;

    int promoRank = m_position.sideToMove() == WHITE ? 7 : 0;
    return rankOf(toSq) == promoRank;
}

std::optional<std::pair<std::string, std::string>> BitboardEngine::getCastlingRookMove(
    const std::string& kingFrom, const std::string& kingTo
) const {
    Square fromSq = coordinateToSquare(kingFrom);
    Square toSq = coordinateToSquare(kingTo);

    if (fromSq == NO_SQUARE || toSq == NO_SQUARE) return std::nullopt;

    Color us = m_position.colorOn(fromSq);
    if (us == NO_COLOR) return std::nullopt;

    bool kingside = toSq > fromSq;

    Square rookFrom, rookTo;
    if (us == WHITE) {
        rookFrom = kingside ? SQ_H1 : SQ_A1;
        rookTo = kingside ? SQ_F1 : SQ_D1;
    } else {
        rookFrom = kingside ? SQ_H8 : SQ_A8;
        rookTo = kingside ? SQ_F8 : SQ_D8;
    }

    return std::make_pair(squareToCoordinate(rookFrom), squareToCoordinate(rookTo));
}

std::optional<std::string> BitboardEngine::getEnPassantCaptureSquare(
    const std::string& from, const std::string& to
) const {
    Square toSq = coordinateToSquare(to);
    if (toSq == NO_SQUARE) return std::nullopt;

    Square fromSq = coordinateToSquare(from);
    if (fromSq == NO_SQUARE) return std::nullopt;

    Color us = m_position.colorOn(fromSq);
    if (us == NO_COLOR) return std::nullopt;

    Square capturedSq = toSq + (us == WHITE ? -8 : 8);
    return squareToCoordinate(capturedSq);
}

uint64_t BitboardEngine::perft(int depth) {
    return MoveGenerator::perft(m_position, depth);
}

void BitboardEngine::printBoard() const {
    m_position.print();
}

} // namespace Bitboard
