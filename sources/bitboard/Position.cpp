#include "../../headers/bitboard/Position.h"
#include "../../headers/bitboard/BitboardUtils.h"
#include <sstream>
#include <iostream>
#include <algorithm>

namespace Bitboard {

Position::Position() {
    m_state = &m_rootState;
    clear();
}

void Position::clear() {
    std::fill(m_byType.begin(), m_byType.end(), 0);
    std::fill(m_byColor.begin(), m_byColor.end(), 0);
    std::fill(m_board.begin(), m_board.end(), NO_PIECE_TYPE);
    std::fill(m_boardColor.begin(), m_boardColor.end(), NO_COLOR);
    std::fill(m_kingSquare.begin(), m_kingSquare.end(), NO_SQUARE);

    m_sideToMove = WHITE;
    m_fullmoveNumber = 1;

    m_rootState.castlingRights = NO_CASTLING;
    m_rootState.enPassantSquare = NO_SQUARE;
    m_rootState.halfmoveClock = 0;
    m_rootState.capturedPiece = NO_PIECE_TYPE;
    m_rootState.hash = 0;
    m_rootState.checkers = 0;
    m_rootState.pinned = 0;
    m_rootState.blockers = 0;
    m_rootState.previous = nullptr;

    m_state = &m_rootState;
    m_history.clear();
}

void Position::reset() {
    setFromFEN(START_FEN);
}

void Position::putPiece(PieceType pt, Color c, Square sq) {
    m_board[sq] = pt;
    m_boardColor[sq] = c;
    m_byType[pt] |= squareBB(sq);
    m_byColor[c] |= squareBB(sq);

    if (pt == KING) {
        m_kingSquare[c] = sq;
    }
}

void Position::removePiece(Square sq) {
    PieceType pt = m_board[sq];
    Color c = m_boardColor[sq];

    if (pt == NO_PIECE_TYPE) return;

    m_byType[pt] &= ~squareBB(sq);
    m_byColor[c] &= ~squareBB(sq);
    m_board[sq] = NO_PIECE_TYPE;
    m_boardColor[sq] = NO_COLOR;
}

void Position::movePiece(Square from, Square to) {
    PieceType pt = m_board[from];
    Color c = m_boardColor[from];
    Bitboard fromTo = squareBB(from) | squareBB(to);

    m_byType[pt] ^= fromTo;
    m_byColor[c] ^= fromTo;
    m_board[to] = pt;
    m_boardColor[to] = c;
    m_board[from] = NO_PIECE_TYPE;
    m_boardColor[from] = NO_COLOR;

    if (pt == KING) {
        m_kingSquare[c] = to;
    }
}

void Position::setFromFEN(const std::string& fen) {
    clear();

    std::istringstream ss(fen);
    std::string token;

    // 1. Piece placement
    ss >> token;
    Square sq = SQ_A8;
    for (char c : token) {
        if (c == '/') {
            sq -= 16;  // Move to start of next rank down
        } else if (c >= '1' && c <= '8') {
            sq += (c - '0');  // Skip empty squares
        } else {
            auto [pt, color] = charToPiece(c);
            if (pt != NO_PIECE_TYPE) {
                putPiece(pt, color, sq);
                ++sq;
            }
        }
    }

    // 2. Side to move
    ss >> token;
    m_sideToMove = (token == "w") ? WHITE : BLACK;

    // 3. Castling rights
    ss >> token;
    m_state->castlingRights = NO_CASTLING;
    for (char c : token) {
        switch (c) {
            case 'K': m_state->castlingRights |= WHITE_OO; break;
            case 'Q': m_state->castlingRights |= WHITE_OOO; break;
            case 'k': m_state->castlingRights |= BLACK_OO; break;
            case 'q': m_state->castlingRights |= BLACK_OOO; break;
            default: break;
        }
    }

    // 4. En passant square
    ss >> token;
    if (token != "-") {
        m_state->enPassantSquare = coordinateToSquare(token);
    } else {
        m_state->enPassantSquare = NO_SQUARE;
    }

    // 5. Halfmove clock
    if (ss >> token) {
        m_state->halfmoveClock = std::stoi(token);
    }

    // 6. Fullmove number
    if (ss >> token) {
        m_fullmoveNumber = std::stoi(token);
    }

    updateState();
}

std::string Position::toFEN() const {
    std::ostringstream fen;

    // 1. Piece placement
    for (int rank = 7; rank >= 0; --rank) {
        int empty = 0;
        for (int file = 0; file < 8; ++file) {
            Square sq = makeSquare(file, rank);
            PieceType pt = m_board[sq];
            if (pt == NO_PIECE_TYPE) {
                ++empty;
            } else {
                if (empty > 0) {
                    fen << empty;
                    empty = 0;
                }
                fen << pieceToChar(pt, m_boardColor[sq]);
            }
        }
        if (empty > 0) {
            fen << empty;
        }
        if (rank > 0) {
            fen << '/';
        }
    }

    // 2. Side to move
    fen << ' ' << (m_sideToMove == WHITE ? 'w' : 'b');

    // 3. Castling rights
    fen << ' ';
    if (m_state->castlingRights == NO_CASTLING) {
        fen << '-';
    } else {
        if (m_state->castlingRights & WHITE_OO) fen << 'K';
        if (m_state->castlingRights & WHITE_OOO) fen << 'Q';
        if (m_state->castlingRights & BLACK_OO) fen << 'k';
        if (m_state->castlingRights & BLACK_OOO) fen << 'q';
    }

    // 4. En passant square
    fen << ' ';
    if (m_state->enPassantSquare == NO_SQUARE) {
        fen << '-';
    } else {
        fen << squareToCoordinate(m_state->enPassantSquare);
    }

    // 5. Halfmove clock
    fen << ' ' << m_state->halfmoveClock;

    // 6. Fullmove number
    fen << ' ' << m_fullmoveNumber;

    return fen.str();
}

// Piece access

Bitboard Position::pieces() const {
    return m_byColor[WHITE] | m_byColor[BLACK];
}

Bitboard Position::pieces(Color c) const {
    return m_byColor[c];
}

Bitboard Position::pieces(PieceType pt) const {
    return m_byType[pt];
}

Bitboard Position::pieces(Color c, PieceType pt) const {
    return m_byColor[c] & m_byType[pt];
}

Bitboard Position::pieces(PieceType pt1, PieceType pt2) const {
    return m_byType[pt1] | m_byType[pt2];
}

Bitboard Position::pieces(Color c, PieceType pt1, PieceType pt2) const {
    return m_byColor[c] & (m_byType[pt1] | m_byType[pt2]);
}

PieceType Position::pieceOn(Square sq) const {
    return m_board[sq];
}

Color Position::colorOn(Square sq) const {
    return m_boardColor[sq];
}

bool Position::isEmpty(Square sq) const {
    return m_board[sq] == NO_PIECE_TYPE;
}

Square Position::kingSquare(Color c) const {
    return m_kingSquare[c];
}

int Position::pieceCount(Color c, PieceType pt) const {
    return popcount(pieces(c, pt));
}

int Position::pieceCount(PieceType pt) const {
    return popcount(pieces(pt));
}

// Castling

bool Position::canCastle(CastlingRights cr) const {
    return (m_state->castlingRights & cr) != NO_CASTLING;
}

bool Position::canCastle(Color c) const {
    return canCastle(c == WHITE ? WHITE_CASTLING : BLACK_CASTLING);
}

void Position::setCastlingRights(CastlingRights cr) {
    m_state->castlingRights = cr;
}

void Position::setEnPassantSquare(Square sq) {
    m_state->enPassantSquare = sq;
}

// Attack information

Bitboard Position::attackersTo(Square sq) const {
    return attackersTo(sq, pieces());
}

Bitboard Position::attackersTo(Square sq, Bitboard occupied) const {
    return (pawnAttacks(WHITE, sq) & pieces(BLACK, PAWN))
         | (pawnAttacks(BLACK, sq) & pieces(WHITE, PAWN))
         | (knightAttacks(sq) & pieces(KNIGHT))
         | (bishopAttacks(sq, occupied) & pieces(BISHOP, QUEEN))
         | (rookAttacks(sq, occupied) & pieces(ROOK, QUEEN))
         | (kingAttacks(sq) & pieces(KING));
}

Bitboard Position::attackersTo(Square sq, Color by) const {
    return attackersTo(sq) & pieces(by);
}

bool Position::isAttacked(Square sq, Color by) const {
    return attackersTo(sq, by) != 0;
}

Bitboard Position::attackedBy(Color c) const {
    Bitboard attacked = 0;
    Bitboard occupied = pieces();

    // Pawns
    attacked |= MagicBitboards::pawnAttacks(c, pieces(c, PAWN));

    // Knights
    Bitboard knights = pieces(c, KNIGHT);
    while (knights) {
        attacked |= knightAttacks(popLsb(knights));
    }

    // Bishops and Queens (diagonal)
    Bitboard bishops = pieces(c, BISHOP, QUEEN);
    while (bishops) {
        attacked |= bishopAttacks(popLsb(bishops), occupied);
    }

    // Rooks and Queens (orthogonal)
    Bitboard rooks = pieces(c, ROOK, QUEEN);
    while (rooks) {
        attacked |= rookAttacks(popLsb(rooks), occupied);
    }

    // King
    attacked |= kingAttacks(kingSquare(c));

    return attacked;
}

Bitboard Position::attackedBy(Color c, PieceType pt) const {
    Bitboard attacked = 0;
    Bitboard occupied = pieces();

    switch (pt) {
        case PAWN:
            attacked = MagicBitboards::pawnAttacks(c, pieces(c, PAWN));
            break;
        case KNIGHT: {
            Bitboard knights = pieces(c, KNIGHT);
            while (knights) {
                attacked |= knightAttacks(popLsb(knights));
            }
            break;
        }
        case BISHOP: {
            Bitboard bishops = pieces(c, BISHOP);
            while (bishops) {
                attacked |= bishopAttacks(popLsb(bishops), occupied);
            }
            break;
        }
        case ROOK: {
            Bitboard rooks = pieces(c, ROOK);
            while (rooks) {
                attacked |= rookAttacks(popLsb(rooks), occupied);
            }
            break;
        }
        case QUEEN: {
            Bitboard queens = pieces(c, QUEEN);
            while (queens) {
                attacked |= queenAttacks(popLsb(queens), occupied);
            }
            break;
        }
        case KING:
            attacked = kingAttacks(kingSquare(c));
            break;
        default:
            break;
    }

    return attacked;
}

Bitboard Position::sliderBlockers(Bitboard sliders, Square sq, Bitboard& pinners) const {
    Bitboard blockers = 0;
    pinners = 0;

    // Potential pinners are sliders that could attack the square on an empty board
    Bitboard snipers = ((MagicBitboards::pseudoRookAttacks(sq) & pieces(ROOK, QUEEN))
                      | (MagicBitboards::pseudoBishopAttacks(sq) & pieces(BISHOP, QUEEN))) & sliders;

    Bitboard occupied = pieces() ^ snipers;

    while (snipers) {
        Square sniperSq = popLsb(snipers);
        Bitboard between = lineBetween(sq, sniperSq) & occupied;

        // If exactly one piece between, it's a blocker (potential pin)
        if (between && !moreThanOne(between)) {
            blockers |= between;
            if (between & pieces(colorOn(sq))) {
                pinners |= squareBB(sniperSq);
            }
        }
    }

    return blockers;
}

Bitboard Position::pinnedPieces(Color c) const {
    Bitboard pinners;
    return sliderBlockers(pieces(~c), kingSquare(c), pinners) & pieces(c);
}

Bitboard Position::blockersForKing(Color c) const {
    Bitboard pinners;
    return sliderBlockers(pieces(~c), kingSquare(c), pinners);
}

// State update

void Position::updateState() {
    computeHash();
    updateCheckers();
    updatePinned();
}

void Position::computeHash() {
    uint64_t h = 0;

    // Piece positions
    for (Square sq = 0; sq < 64; ++sq) {
        PieceType pt = m_board[sq];
        if (pt != NO_PIECE_TYPE) {
            h ^= Zobrist::pieceSquare(pt, m_boardColor[sq], sq);
        }
    }

    // Side to move
    if (m_sideToMove == BLACK) {
        h ^= Zobrist::sideToMove();
    }

    // Castling rights
    h ^= Zobrist::castling(m_state->castlingRights);

    // En passant
    if (m_state->enPassantSquare != NO_SQUARE) {
        h ^= Zobrist::enPassant(m_state->enPassantSquare);
    }

    m_state->hash = h;
}

void Position::updateCheckers() {
    m_state->checkers = attackersTo(kingSquare(m_sideToMove), ~m_sideToMove);
}

void Position::updatePinned() {
    Bitboard pinners;
    m_state->blockers = sliderBlockers(pieces(~m_sideToMove), kingSquare(m_sideToMove), pinners);
    m_state->pinned = m_state->blockers & pieces(m_sideToMove);
}

// Move execution

void Position::makeMove(Move m, StateInfo& newState) {
    // Copy current state
    newState = *m_state;
    newState.previous = m_state;
    m_state = &newState;

    Square from = moveFrom(m);
    Square to = moveTo(m);
    PieceType pt = m_board[from];
    PieceType captured = m_board[to];
    Color us = m_sideToMove;
    Color them = ~us;

    // Save captured piece for unmake
    m_state->capturedPiece = captured;

    // Update hash for captured piece
    if (captured != NO_PIECE_TYPE) {
        m_state->hash ^= Zobrist::pieceSquare(captured, them, to);
    }

    // Update hash for en passant
    if (m_state->enPassantSquare != NO_SQUARE) {
        m_state->hash ^= Zobrist::enPassant(m_state->enPassantSquare);
    }

    // Reset en passant
    m_state->enPassantSquare = NO_SQUARE;

    // Update halfmove clock
    if (pt == PAWN || captured != NO_PIECE_TYPE) {
        m_state->halfmoveClock = 0;
    } else {
        m_state->halfmoveClock++;
    }

    // Handle different move types
    MoveType type = moveType(m);

    if (type == CASTLING) {
        // Move king
        m_state->hash ^= Zobrist::pieceSquare(KING, us, from);
        m_state->hash ^= Zobrist::pieceSquare(KING, us, to);
        movePiece(from, to);

        // Move rook
        Square rookFrom, rookTo;
        if (to > from) {  // Kingside
            rookFrom = us == WHITE ? SQ_H1 : SQ_H8;
            rookTo = us == WHITE ? SQ_F1 : SQ_F8;
        } else {  // Queenside
            rookFrom = us == WHITE ? SQ_A1 : SQ_A8;
            rookTo = us == WHITE ? SQ_D1 : SQ_D8;
        }

        m_state->hash ^= Zobrist::pieceSquare(ROOK, us, rookFrom);
        m_state->hash ^= Zobrist::pieceSquare(ROOK, us, rookTo);
        movePiece(rookFrom, rookTo);
    }
    else if (type == EN_PASSANT) {
        Square capturedSq = to + (us == WHITE ? -8 : 8);
        m_state->hash ^= Zobrist::pieceSquare(PAWN, them, capturedSq);
        removePiece(capturedSq);

        m_state->hash ^= Zobrist::pieceSquare(PAWN, us, from);
        m_state->hash ^= Zobrist::pieceSquare(PAWN, us, to);
        movePiece(from, to);
    }
    else if (type == PROMOTION) {
        PieceType promoPiece = movePromotion(m);

        if (captured != NO_PIECE_TYPE) {
            removePiece(to);
        }

        m_state->hash ^= Zobrist::pieceSquare(PAWN, us, from);
        removePiece(from);

        m_state->hash ^= Zobrist::pieceSquare(promoPiece, us, to);
        putPiece(promoPiece, us, to);
    }
    else {  // NORMAL
        if (captured != NO_PIECE_TYPE) {
            removePiece(to);
        }

        m_state->hash ^= Zobrist::pieceSquare(pt, us, from);
        m_state->hash ^= Zobrist::pieceSquare(pt, us, to);
        movePiece(from, to);

        // Set en passant square for double pawn push
        if (pt == PAWN && std::abs(to - from) == 16) {
            Square epSquare = static_cast<Square>((from + to) / 2);
            // Only set if opponent pawn can capture
            if (pawnAttacks(us, epSquare) & pieces(them, PAWN)) {
                m_state->enPassantSquare = epSquare;
                m_state->hash ^= Zobrist::enPassant(epSquare);
            }
        }
    }

    // Update castling rights
    CastlingRights oldCastling = m_state->castlingRights;

    // King moves remove all castling rights for that side
    if (pt == KING) {
        m_state->castlingRights &= (us == WHITE ? BLACK_CASTLING : WHITE_CASTLING);
    }

    // Rook moves or captures remove specific castling rights
    if (from == SQ_A1 || to == SQ_A1) m_state->castlingRights &= ~WHITE_OOO;
    if (from == SQ_H1 || to == SQ_H1) m_state->castlingRights &= ~WHITE_OO;
    if (from == SQ_A8 || to == SQ_A8) m_state->castlingRights &= ~BLACK_OOO;
    if (from == SQ_H8 || to == SQ_H8) m_state->castlingRights &= ~BLACK_OO;

    // Update hash for castling rights change
    if (oldCastling != m_state->castlingRights) {
        m_state->hash ^= Zobrist::castling(oldCastling);
        m_state->hash ^= Zobrist::castling(m_state->castlingRights);
    }

    // Switch side to move
    m_sideToMove = them;
    m_state->hash ^= Zobrist::sideToMove();

    // Update fullmove number
    if (us == BLACK) {
        m_fullmoveNumber++;
    }

    // Update checkers and pinned pieces
    updateCheckers();
    updatePinned();

    // Add to history for repetition detection
    m_history.push_back(m_state->hash);
}

void Position::unmakeMove(Move m) {
    m_sideToMove = ~m_sideToMove;
    Color us = m_sideToMove;
    Color them = ~us;

    Square from = moveFrom(m);
    Square to = moveTo(m);
    MoveType type = moveType(m);
    PieceType captured = m_state->capturedPiece;

    if (type == CASTLING) {
        // Unmove king
        movePiece(to, from);

        // Unmove rook
        Square rookFrom, rookTo;
        if (to > from) {  // Kingside
            rookFrom = us == WHITE ? SQ_H1 : SQ_H8;
            rookTo = us == WHITE ? SQ_F1 : SQ_F8;
        } else {  // Queenside
            rookFrom = us == WHITE ? SQ_A1 : SQ_A8;
            rookTo = us == WHITE ? SQ_D1 : SQ_D8;
        }
        movePiece(rookTo, rookFrom);
    }
    else if (type == EN_PASSANT) {
        movePiece(to, from);

        // Restore captured pawn
        Square capturedSq = to + (us == WHITE ? -8 : 8);
        putPiece(PAWN, them, capturedSq);
    }
    else if (type == PROMOTION) {
        removePiece(to);
        putPiece(PAWN, us, from);

        if (captured != NO_PIECE_TYPE) {
            putPiece(captured, them, to);
        }
    }
    else {  // NORMAL
        movePiece(to, from);

        if (captured != NO_PIECE_TYPE) {
            putPiece(captured, them, to);
        }
    }

    // Restore fullmove number
    if (us == BLACK) {
        m_fullmoveNumber--;
    }

    // Restore previous state
    m_state = m_state->previous;

    // Remove from history
    if (!m_history.empty()) {
        m_history.pop_back();
    }
}

void Position::makeNullMove(StateInfo& newState) {
    newState = *m_state;
    newState.previous = m_state;
    m_state = &newState;

    if (m_state->enPassantSquare != NO_SQUARE) {
        m_state->hash ^= Zobrist::enPassant(m_state->enPassantSquare);
        m_state->enPassantSquare = NO_SQUARE;
    }

    m_sideToMove = ~m_sideToMove;
    m_state->hash ^= Zobrist::sideToMove();

    updateCheckers();
    updatePinned();

    m_state->halfmoveClock++;
}

void Position::unmakeNullMove() {
    m_sideToMove = ~m_sideToMove;
    m_state = m_state->previous;
}

// Move validation

bool Position::isCapture(Move m) const {
    return m_board[moveTo(m)] != NO_PIECE_TYPE || moveType(m) == EN_PASSANT;
}

bool Position::isPseudoLegal(Move m) const {
    if (m == NO_MOVE) return false;

    Square from = moveFrom(m);
    Square to = moveTo(m);
    PieceType pt = m_board[from];

    // Must have a piece to move
    if (pt == NO_PIECE_TYPE) return false;

    // Must be our piece
    if (m_boardColor[from] != m_sideToMove) return false;

    // Can't capture own piece
    if (m_boardColor[to] == m_sideToMove) return false;

    MoveType type = moveType(m);

    if (type == CASTLING) {
        // Detailed castling validation
        if (pt != KING) return false;
        if (inCheck()) return false;

        bool kingside = to > from;
        CastlingRights cr = (m_sideToMove == WHITE)
            ? (kingside ? WHITE_OO : WHITE_OOO)
            : (kingside ? BLACK_OO : BLACK_OOO);

        if (!canCastle(cr)) return false;

        Square rookSq = kingside
            ? (m_sideToMove == WHITE ? SQ_H1 : SQ_H8)
            : (m_sideToMove == WHITE ? SQ_A1 : SQ_A8);

        if (m_board[rookSq] != ROOK || m_boardColor[rookSq] != m_sideToMove) return false;

        // Check path is clear
        Bitboard path = lineBetween(from, rookSq);
        if (path & pieces()) return false;

        // Check king doesn't pass through attacked squares
        int dir = kingside ? 1 : -1;
        for (Square sq = from; sq != to; sq += dir) {
            if (isAttacked(sq, ~m_sideToMove)) return false;
        }
        if (isAttacked(to, ~m_sideToMove)) return false;

        return true;
    }

    if (pt == PAWN) {
        int direction = (m_sideToMove == WHITE) ? 8 : -8;
        int startRank = (m_sideToMove == WHITE) ? 1 : 6;
        int promoRank = (m_sideToMove == WHITE) ? 7 : 0;

        if (type == EN_PASSANT) {
            return to == m_state->enPassantSquare
                && (pawnAttacks(m_sideToMove, from) & squareBB(to));
        }

        if (type == PROMOTION) {
            if (rankOf(to) != promoRank) return false;
        } else if (rankOf(to) == promoRank) {
            return false;  // Must be promotion
        }

        // Captures
        if (fileOf(from) != fileOf(to)) {
            return (pawnAttacks(m_sideToMove, from) & squareBB(to))
                && m_board[to] != NO_PIECE_TYPE;
        }

        // Single push
        if (to == from + direction) {
            return m_board[to] == NO_PIECE_TYPE;
        }

        // Double push
        if (to == from + 2 * direction && rankOf(from) == startRank) {
            return m_board[to] == NO_PIECE_TYPE
                && m_board[from + direction] == NO_PIECE_TYPE;
        }

        return false;
    }

    // Other pieces - check if move is in attack set
    Bitboard attacks;
    switch (pt) {
        case KNIGHT: attacks = knightAttacks(from); break;
        case BISHOP: attacks = bishopAttacks(from, pieces()); break;
        case ROOK:   attacks = rookAttacks(from, pieces()); break;
        case QUEEN:  attacks = queenAttacks(from, pieces()); break;
        case KING:   attacks = kingAttacks(from); break;
        default:     return false;
    }

    return attacks & squareBB(to);
}

bool Position::isLegal(Move m) const {
    if (!isPseudoLegal(m)) return false;

    Square from = moveFrom(m);
    Square to = moveTo(m);
    Color us = m_sideToMove;
    Square ksq = kingSquare(us);

    // Castling legality already checked in isPseudoLegal
    if (moveType(m) == CASTLING) return true;

    // En passant: need to check for discovered check
    if (moveType(m) == EN_PASSANT) {
        Square capturedSq = to + (us == WHITE ? -8 : 8);
        Bitboard occupied = (pieces() ^ squareBB(from) ^ squareBB(capturedSq)) | squareBB(to);

        // Check if king is attacked with new occupancy
        return !(bishopAttacks(ksq, occupied) & pieces(~us, BISHOP, QUEEN))
            && !(rookAttacks(ksq, occupied) & pieces(~us, ROOK, QUEEN));
    }

    // King moves: check if destination is attacked
    if (m_board[from] == KING) {
        return !isAttacked(to, ~us);
    }

    // If the piece is pinned, it can only move along the pin line
    if (m_state->pinned & squareBB(from)) {
        return lineThrough(ksq, from) & squareBB(to);
    }

    // If in check, must block or capture the checker
    if (inCheck()) {
        Bitboard checkers = m_state->checkers;

        // Double check: only king moves are legal (already handled above)
        if (moreThanOne(checkers)) return false;

        Square checkerSq = lsb(checkers);

        // Must capture the checker or block it
        if (squareBB(to) & checkers) return true;  // Capture

        // Block: move must be between king and checker
        return lineBetween(ksq, checkerSq) & squareBB(to);
    }

    return true;
}

bool Position::givesCheck(Move m) const {
    Square from = moveFrom(m);
    Square to = moveTo(m);
    PieceType pt = m_board[from];
    Square theirKing = kingSquare(~m_sideToMove);

    // Direct check from destination square
    Bitboard attacks;
    switch (pt) {
        case PAWN:   attacks = pawnAttacks(m_sideToMove, to); break;
        case KNIGHT: attacks = knightAttacks(to); break;
        case BISHOP: attacks = bishopAttacks(to, pieces() ^ squareBB(from)); break;
        case ROOK:   attacks = rookAttacks(to, pieces() ^ squareBB(from)); break;
        case QUEEN:  attacks = queenAttacks(to, pieces() ^ squareBB(from)); break;
        case KING:   attacks = 0; break;  // King can't give direct check
        default:     attacks = 0; break;
    }

    if (attacks & squareBB(theirKing)) return true;

    // Discovered check
    Bitboard blockers = blockersForKing(~m_sideToMove);
    if (blockers & squareBB(from)) {
        // Moving off the line might give discovered check
        if (!(lineThrough(theirKing, from) & squareBB(to))) {
            return true;
        }
    }

    // Special cases
    MoveType type = moveType(m);

    if (type == CASTLING) {
        // Check from rook
        Square rookTo = to > from
            ? (m_sideToMove == WHITE ? SQ_F1 : SQ_F8)
            : (m_sideToMove == WHITE ? SQ_D1 : SQ_D8);
        return rookAttacks(rookTo, pieces() ^ squareBB(from) ^ squareBB(to)) & squareBB(theirKing);
    }

    if (type == EN_PASSANT) {
        Square capturedSq = to + (m_sideToMove == WHITE ? -8 : 8);
        Bitboard occupied = (pieces() ^ squareBB(from) ^ squareBB(capturedSq)) | squareBB(to);
        return (bishopAttacks(theirKing, occupied) & pieces(m_sideToMove, BISHOP, QUEEN))
            || (rookAttacks(theirKing, occupied) & pieces(m_sideToMove, ROOK, QUEEN));
    }

    if (type == PROMOTION) {
        PieceType promoPt = movePromotion(m);
        Bitboard occupied = (pieces() ^ squareBB(from)) | squareBB(to);
        switch (promoPt) {
            case KNIGHT: return knightAttacks(to) & squareBB(theirKing);
            case BISHOP: return bishopAttacks(to, occupied) & squareBB(theirKing);
            case ROOK:   return rookAttacks(to, occupied) & squareBB(theirKing);
            case QUEEN:  return queenAttacks(to, occupied) & squareBB(theirKing);
            default:     break;
        }
    }

    return false;
}

// Game state

bool Position::hasRepeated() const {
    // Check for threefold repetition
    int count = 0;
    uint64_t currentHash = m_state->hash;

    for (auto it = m_history.rbegin(); it != m_history.rend(); ++it) {
        if (*it == currentHash) {
            if (++count >= 2) return true;  // Current position + 2 repeats = 3 total
        }
    }

    return false;
}

bool Position::hasInsufficientMaterial() const {
    // Pawns, rooks, or queens: always sufficient
    if (pieces(PAWN) || pieces(ROOK) || pieces(QUEEN)) return false;

    // Count bishops and knights
    int whiteBishops = popcount(pieces(WHITE, BISHOP));
    int blackBishops = popcount(pieces(BLACK, BISHOP));
    int whiteKnights = popcount(pieces(WHITE, KNIGHT));
    int blackKnights = popcount(pieces(BLACK, KNIGHT));

    int whitePieces = whiteBishops + whiteKnights;
    int blackPieces = blackBishops + blackKnights;

    // King vs King
    if (whitePieces == 0 && blackPieces == 0) return true;

    // King + minor vs King
    if ((whitePieces == 1 && blackPieces == 0) ||
        (whitePieces == 0 && blackPieces == 1)) return true;

    // King + Bishop vs King + Bishop (same color bishops)
    if (whitePieces == 1 && blackPieces == 1 &&
        whiteBishops == 1 && blackBishops == 1) {
        // Check if bishops are on same color
        Square wb = lsb(pieces(WHITE, BISHOP));
        Square bb = lsb(pieces(BLACK, BISHOP));
        if (((fileOf(wb) + rankOf(wb)) & 1) == ((fileOf(bb) + rankOf(bb)) & 1)) {
            return true;
        }
    }

    return false;
}

bool Position::isDraw() const {
    // 50-move rule
    if (m_state->halfmoveClock >= 100) return true;

    // Repetition
    if (hasRepeated()) return true;

    // Insufficient material
    if (hasInsufficientMaterial()) return true;

    return false;
}

bool Position::isCheckmate() const {
    // Must be in check and have no legal moves
    // This is a placeholder - proper implementation in MoveGenerator
    return inCheck();  // Will be refined when integrated with MoveGenerator
}

bool Position::isStalemate() const {
    // Not in check and no legal moves
    // This is a placeholder - proper implementation in MoveGenerator
    return !inCheck();  // Will be refined when integrated with MoveGenerator
}

// Debug

void Position::print() const {
    std::cout << "\n +---+---+---+---+---+---+---+---+\n";

    for (int rank = 7; rank >= 0; --rank) {
        std::cout << " |";
        for (int file = 0; file < 8; ++file) {
            Square sq = makeSquare(file, rank);
            PieceType pt = m_board[sq];
            if (pt == NO_PIECE_TYPE) {
                std::cout << "   |";
            } else {
                std::cout << ' ' << pieceToChar(pt, m_boardColor[sq]) << " |";
            }
        }
        std::cout << " " << (rank + 1) << "\n +---+---+---+---+---+---+---+---+\n";
    }

    std::cout << "   a   b   c   d   e   f   g   h\n\n";
    std::cout << "FEN: " << toFEN() << "\n";
    std::cout << "Hash: " << std::hex << m_state->hash << std::dec << "\n";
}

bool Position::isValid() const {
    // Basic validity checks
    if (popcount(pieces(WHITE, KING)) != 1) return false;
    if (popcount(pieces(BLACK, KING)) != 1) return false;

    // Opponent king should not be in check
    if (isAttacked(kingSquare(~m_sideToMove), m_sideToMove)) return false;

    return true;
}

} // namespace Bitboard
