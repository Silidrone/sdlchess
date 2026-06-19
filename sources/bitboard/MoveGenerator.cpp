#include "../../headers/bitboard/MoveGenerator.h"
#include "../../headers/bitboard/BitboardUtils.h"

namespace Bitboard {

void MoveGenerator::addPawnMoves(MoveList& moves, Bitboard pawns, int delta, MoveType type) {
    while (pawns) {
        Square to = popLsb(pawns);
        Square from = static_cast<Square>(to - delta);
        moves.push(makeMove(from, to, type));
    }
}

void MoveGenerator::addPromotions(MoveList& moves, Square from, Square to) {
    moves.push(makeMove(from, to, PROMOTION, QUEEN));
    moves.push(makeMove(from, to, PROMOTION, ROOK));
    moves.push(makeMove(from, to, PROMOTION, BISHOP));
    moves.push(makeMove(from, to, PROMOTION, KNIGHT));
}

void MoveGenerator::addPromotionCaptures(MoveList& moves, Square from, Square to) {
    // Same as addPromotions - captures are determined by target square content
    addPromotions(moves, from, to);
}

void MoveGenerator::generatePawnMoves(const Position& pos, MoveList& moves, Bitboard targets, bool capturesOnly) {
    Color us = pos.sideToMove();
    Color them = ~us;
    Bitboard ourPawns = pos.pieces(us, PAWN);
    Bitboard occupied = pos.pieces();
    Bitboard enemies = pos.pieces(them);

    int pushDir = (us == WHITE) ? 8 : -8;
    Bitboard promoRank = (us == WHITE) ? RANK_8 : RANK_1;
    Bitboard startRank = (us == WHITE) ? RANK_2 : RANK_7;
    Bitboard rank3 = (us == WHITE) ? RANK_3 : RANK_6;

    Bitboard emptySquares = ~occupied;

    if (!capturesOnly) {
        // Single pawn pushes (non-promotion)
        Bitboard singlePush = pawnPush(ourPawns, us) & emptySquares;
        Bitboard singlePushNoPromo = singlePush & ~promoRank & targets;

        while (singlePushNoPromo) {
            Square to = popLsb(singlePushNoPromo);
            moves.push(makeMove(static_cast<Square>(to - pushDir), to));
        }

        // Double pawn pushes
        Bitboard doublePush = pawnPush(singlePush & rank3, us) & emptySquares & targets;
        while (doublePush) {
            Square to = popLsb(doublePush);
            moves.push(makeMove(static_cast<Square>(to - 2 * pushDir), to));
        }

        // Promotions (quiet)
        Bitboard promotions = singlePush & promoRank & targets;
        while (promotions) {
            Square to = popLsb(promotions);
            Square from = static_cast<Square>(to - pushDir);
            addPromotions(moves, from, to);
        }
    }

    // Captures (including promotion captures)
    Bitboard captureTargets = capturesOnly ? (enemies & targets) : enemies;

    // Left captures
    Bitboard leftCaptures = pawnAttacksLeft(ourPawns, us) & captureTargets;
    Bitboard leftCapturesNoPromo = leftCaptures & ~promoRank;
    Bitboard leftCapturesPromo = leftCaptures & promoRank;

    int leftDelta = (us == WHITE) ? 7 : -9;
    while (leftCapturesNoPromo) {
        Square to = popLsb(leftCapturesNoPromo);
        moves.push(makeMove(static_cast<Square>(to - leftDelta), to));
    }
    while (leftCapturesPromo) {
        Square to = popLsb(leftCapturesPromo);
        addPromotions(moves, static_cast<Square>(to - leftDelta), to);
    }

    // Right captures
    Bitboard rightCaptures = pawnAttacksRight(ourPawns, us) & captureTargets;
    Bitboard rightCapturesNoPromo = rightCaptures & ~promoRank;
    Bitboard rightCapturesPromo = rightCaptures & promoRank;

    int rightDelta = (us == WHITE) ? 9 : -7;
    while (rightCapturesNoPromo) {
        Square to = popLsb(rightCapturesNoPromo);
        moves.push(makeMove(static_cast<Square>(to - rightDelta), to));
    }
    while (rightCapturesPromo) {
        Square to = popLsb(rightCapturesPromo);
        addPromotions(moves, static_cast<Square>(to - rightDelta), to);
    }

    // En passant
    Square epSquare = pos.enPassantSquare();
    if (epSquare != NO_SQUARE) {
        Bitboard epPawns = pawnAttacks(them, epSquare) & ourPawns;
        while (epPawns) {
            Square from = popLsb(epPawns);
            moves.push(makeMove(from, epSquare, EN_PASSANT));
        }
    }
}

void MoveGenerator::generateKnightMoves(const Position& pos, MoveList& moves, Bitboard targets) {
    Color us = pos.sideToMove();
    Bitboard knights = pos.pieces(us, KNIGHT);

    while (knights) {
        Square from = popLsb(knights);
        Bitboard attacks = knightAttacks(from) & targets;

        while (attacks) {
            Square to = popLsb(attacks);
            moves.push(makeMove(from, to));
        }
    }
}

void MoveGenerator::generateBishopMoves(const Position& pos, MoveList& moves, Bitboard targets) {
    Color us = pos.sideToMove();
    Bitboard bishops = pos.pieces(us, BISHOP);
    Bitboard occupied = pos.pieces();

    while (bishops) {
        Square from = popLsb(bishops);
        Bitboard attacks = bishopAttacks(from, occupied) & targets;

        while (attacks) {
            Square to = popLsb(attacks);
            moves.push(makeMove(from, to));
        }
    }
}

void MoveGenerator::generateRookMoves(const Position& pos, MoveList& moves, Bitboard targets) {
    Color us = pos.sideToMove();
    Bitboard rooks = pos.pieces(us, ROOK);
    Bitboard occupied = pos.pieces();

    while (rooks) {
        Square from = popLsb(rooks);
        Bitboard attacks = rookAttacks(from, occupied) & targets;

        while (attacks) {
            Square to = popLsb(attacks);
            moves.push(makeMove(from, to));
        }
    }
}

void MoveGenerator::generateQueenMoves(const Position& pos, MoveList& moves, Bitboard targets) {
    Color us = pos.sideToMove();
    Bitboard queens = pos.pieces(us, QUEEN);
    Bitboard occupied = pos.pieces();

    while (queens) {
        Square from = popLsb(queens);
        Bitboard attacks = queenAttacks(from, occupied) & targets;

        while (attacks) {
            Square to = popLsb(attacks);
            moves.push(makeMove(from, to));
        }
    }
}

void MoveGenerator::generateKingMoves(const Position& pos, MoveList& moves, Bitboard targets) {
    Color us = pos.sideToMove();
    Square ksq = pos.kingSquare(us);
    Bitboard attacks = kingAttacks(ksq) & targets;

    while (attacks) {
        Square to = popLsb(attacks);
        moves.push(makeMove(ksq, to));
    }
}

void MoveGenerator::generateCastlingMoves(const Position& pos, MoveList& moves) {
    if (pos.inCheck()) return;

    Color us = pos.sideToMove();
    Square ksq = pos.kingSquare(us);
    Bitboard occupied = pos.pieces();

    // Kingside castling
    CastlingRights ooRight = (us == WHITE) ? WHITE_OO : BLACK_OO;
    if (pos.canCastle(ooRight)) {
        Square rookSq = (us == WHITE) ? SQ_H1 : SQ_H8;
        Square kingTo = (us == WHITE) ? SQ_G1 : SQ_G8;
        Square rookTo = (us == WHITE) ? SQ_F1 : SQ_F8;

        // Check path is clear
        if (!(lineBetween(ksq, rookSq) & occupied)) {
            // Check king doesn't pass through or end in check
            if (!pos.isAttacked(rookTo, ~us) && !pos.isAttacked(kingTo, ~us)) {
                moves.push(makeMove(ksq, kingTo, CASTLING));
            }
        }
    }

    // Queenside castling
    CastlingRights oooRight = (us == WHITE) ? WHITE_OOO : BLACK_OOO;
    if (pos.canCastle(oooRight)) {
        Square rookSq = (us == WHITE) ? SQ_A1 : SQ_A8;
        Square kingTo = (us == WHITE) ? SQ_C1 : SQ_C8;
        Square rookTo = (us == WHITE) ? SQ_D1 : SQ_D8;

        // Check path is clear
        if (!(lineBetween(ksq, rookSq) & occupied)) {
            // Check king doesn't pass through or end in check
            if (!pos.isAttacked(rookTo, ~us) && !pos.isAttacked(kingTo, ~us)) {
                moves.push(makeMove(ksq, kingTo, CASTLING));
            }
        }
    }
}

void MoveGenerator::generatePseudoLegalMoves(const Position& pos, MoveList& moves) {
    Color us = pos.sideToMove();
    Bitboard targets = ~pos.pieces(us);  // Can move to empty squares or enemy pieces

    generatePawnMoves(pos, moves, targets, false);
    generateKnightMoves(pos, moves, targets);
    generateBishopMoves(pos, moves, targets);
    generateRookMoves(pos, moves, targets);
    generateQueenMoves(pos, moves, targets);
    generateKingMoves(pos, moves, targets);
    generateCastlingMoves(pos, moves);
}

void MoveGenerator::generateEvasions(const Position& pos, MoveList& moves) {
    Color us = pos.sideToMove();
    Square ksq = pos.kingSquare(us);
    Bitboard checkers = pos.checkers();

    // King moves (always possible when in check)
    Bitboard kingMoves = kingAttacks(ksq) & ~pos.pieces(us);

    // Remove squares attacked by enemy
    Bitboard occupied = pos.pieces() ^ squareBB(ksq);  // Remove king for slider attacks
    Bitboard enemyAttacks = pos.attackedBy(~us);

    // Also compute slider attacks with king removed
    Bitboard sliders = pos.pieces(~us, BISHOP, QUEEN);
    while (sliders) {
        Square sq = popLsb(sliders);
        enemyAttacks |= bishopAttacks(sq, occupied);
    }
    sliders = pos.pieces(~us, ROOK, QUEEN);
    while (sliders) {
        Square sq = popLsb(sliders);
        enemyAttacks |= rookAttacks(sq, occupied);
    }

    kingMoves &= ~enemyAttacks;

    while (kingMoves) {
        Square to = popLsb(kingMoves);
        moves.push(makeMove(ksq, to));
    }

    // If double check, only king moves are legal
    if (moreThanOne(checkers)) return;

    // Single check: can capture or block
    Square checkerSq = lsb(checkers);
    Bitboard between = lineBetween(ksq, checkerSq);
    Bitboard targets = checkers | between;  // Capture checker or block

    // Generate moves that capture or block
    generatePawnMoves(pos, moves, targets, false);
    generateKnightMoves(pos, moves, targets);
    generateBishopMoves(pos, moves, targets);
    generateRookMoves(pos, moves, targets);
    generateQueenMoves(pos, moves, targets);

    // En passant can also be a valid evasion if it captures the checking pawn
    Square epSquare = pos.enPassantSquare();
    if (epSquare != NO_SQUARE) {
        Square capturedPawnSq = epSquare + (us == WHITE ? -8 : 8);
        if (capturedPawnSq == checkerSq) {
            Bitboard epPawns = pawnAttacks(~us, epSquare) & pos.pieces(us, PAWN);
            while (epPawns) {
                Square from = popLsb(epPawns);
                moves.push(makeMove(from, epSquare, EN_PASSANT));
            }
        }
    }
}

void MoveGenerator::generateLegalMoves(const Position& pos, MoveList& moves) {
    if (pos.inCheck()) {
        generateEvasions(pos, moves);
    } else {
        generatePseudoLegalMoves(pos, moves);
    }

    // Filter out illegal moves
    int legalCount = 0;
    for (int i = 0; i < moves.size(); ++i) {
        if (pos.isLegal(moves[i])) {
            moves[legalCount++] = moves[i];
        }
    }

    // Truncate to legal moves only
    while (moves.size() > legalCount) {
        moves.pop_back();
    }
}

void MoveGenerator::generateCaptures(const Position& pos, MoveList& moves) {
    Color us = pos.sideToMove();
    Bitboard enemies = pos.pieces(~us);

    generatePawnMoves(pos, moves, enemies, true);
    generateKnightMoves(pos, moves, enemies);
    generateBishopMoves(pos, moves, enemies);
    generateRookMoves(pos, moves, enemies);
    generateQueenMoves(pos, moves, enemies);
    generateKingMoves(pos, moves, enemies);

    // Filter out illegal moves
    int legalCount = 0;
    for (int i = 0; i < moves.size(); ++i) {
        if (pos.isLegal(moves[i])) {
            moves[legalCount++] = moves[i];
        }
    }

    while (moves.size() > legalCount) {
        moves.pop_back();
    }
}

void MoveGenerator::generateQuiets(const Position& pos, MoveList& moves) {
    Color us = pos.sideToMove();
    Bitboard empty = ~pos.pieces();

    generatePawnMoves(pos, moves, empty, false);
    generateKnightMoves(pos, moves, empty);
    generateBishopMoves(pos, moves, empty);
    generateRookMoves(pos, moves, empty);
    generateQueenMoves(pos, moves, empty);
    generateKingMoves(pos, moves, empty);
    generateCastlingMoves(pos, moves);

    // Filter out illegal moves
    int legalCount = 0;
    for (int i = 0; i < moves.size(); ++i) {
        if (pos.isLegal(moves[i])) {
            moves[legalCount++] = moves[i];
        }
    }

    while (moves.size() > legalCount) {
        moves.pop_back();
    }
}

bool MoveGenerator::hasLegalMoves(const Position& pos) {
    MoveList moves;
    generateLegalMoves(pos, moves);
    return !moves.empty();
}

void MoveGenerator::getLegalMovesForPiece(const Position& pos, Square sq, MoveList& moves) {
    MoveList allMoves;
    generateLegalMoves(pos, allMoves);

    for (Move m : allMoves) {
        if (moveFrom(m) == sq) {
            moves.push(m);
        }
    }
}

uint64_t MoveGenerator::perft(Position& pos, int depth) {
    if (depth == 0) return 1;

    MoveList moves;
    generateLegalMoves(pos, moves);

    if (depth == 1) return moves.size();

    uint64_t nodes = 0;
    StateInfo st;

    for (Move m : moves) {
        pos.makeMove(m, st);
        nodes += perft(pos, depth - 1);
        pos.unmakeMove(m);
    }

    return nodes;
}

} // namespace Bitboard
