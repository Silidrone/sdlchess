#ifndef BITBOARD_TYPES_H
#define BITBOARD_TYPES_H

#include <cstdint>
#include <string>
#include <array>

namespace Bitboard {

// A bitboard is a 64-bit integer where each bit represents a square
// Bit 0 = a1, Bit 1 = b1, ..., Bit 7 = h1, Bit 8 = a2, ..., Bit 63 = h8
using Bitboard = uint64_t;

// Square index (0-63), using int to allow signed arithmetic
using Square = int;

// Piece types
enum PieceType : int {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
    NO_PIECE_TYPE = 6,
    PIECE_TYPE_COUNT = 6
};

// Colors
enum Color : int {
    WHITE = 0,
    BLACK = 1,
    NO_COLOR = 2,
    COLOR_COUNT = 2
};

// Castling rights as bit flags
enum CastlingRights : int {
    NO_CASTLING = 0,
    WHITE_OO = 1,   // White kingside
    WHITE_OOO = 2,  // White queenside
    BLACK_OO = 4,   // Black kingside
    BLACK_OOO = 8,  // Black queenside
    WHITE_CASTLING = WHITE_OO | WHITE_OOO,
    BLACK_CASTLING = BLACK_OO | BLACK_OOO,
    ALL_CASTLING = WHITE_CASTLING | BLACK_CASTLING
};

// Move encoding (16 bits):
// bits 0-5: from square (0-63)
// bits 6-11: to square (0-63)
// bits 12-13: promotion piece type (0=knight, 1=bishop, 2=rook, 3=queen)
// bits 14-15: move type (0=normal, 1=promotion, 2=en passant, 3=castling)
using Move = uint16_t;

enum MoveType : int {
    NORMAL = 0,
    PROMOTION = 1,
    EN_PASSANT = 2,
    CASTLING = 3
};

// Square constants
constexpr Square SQ_A1 = 0,  SQ_B1 = 1,  SQ_C1 = 2,  SQ_D1 = 3;
constexpr Square SQ_E1 = 4,  SQ_F1 = 5,  SQ_G1 = 6,  SQ_H1 = 7;
constexpr Square SQ_A2 = 8,  SQ_B2 = 9,  SQ_C2 = 10, SQ_D2 = 11;
constexpr Square SQ_E2 = 12, SQ_F2 = 13, SQ_G2 = 14, SQ_H2 = 15;
constexpr Square SQ_A3 = 16, SQ_B3 = 17, SQ_C3 = 18, SQ_D3 = 19;
constexpr Square SQ_E3 = 20, SQ_F3 = 21, SQ_G3 = 22, SQ_H3 = 23;
constexpr Square SQ_A4 = 24, SQ_B4 = 25, SQ_C4 = 26, SQ_D4 = 27;
constexpr Square SQ_E4 = 28, SQ_F4 = 29, SQ_G4 = 30, SQ_H4 = 31;
constexpr Square SQ_A5 = 32, SQ_B5 = 33, SQ_C5 = 34, SQ_D5 = 35;
constexpr Square SQ_E5 = 36, SQ_F5 = 37, SQ_G5 = 38, SQ_H5 = 39;
constexpr Square SQ_A6 = 40, SQ_B6 = 41, SQ_C6 = 42, SQ_D6 = 43;
constexpr Square SQ_E6 = 44, SQ_F6 = 45, SQ_G6 = 46, SQ_H6 = 47;
constexpr Square SQ_A7 = 48, SQ_B7 = 49, SQ_C7 = 50, SQ_D7 = 51;
constexpr Square SQ_E7 = 52, SQ_F7 = 53, SQ_G7 = 54, SQ_H7 = 55;
constexpr Square SQ_A8 = 56, SQ_B8 = 57, SQ_C8 = 58, SQ_D8 = 59;
constexpr Square SQ_E8 = 60, SQ_F8 = 61, SQ_G8 = 62, SQ_H8 = 63;

constexpr Square NO_SQUARE = 64;
constexpr Square SQUARE_COUNT = 64;

constexpr Move NO_MOVE = 0;

// File constants (columns a-h)
constexpr Bitboard FILE_A = 0x0101010101010101ULL;
constexpr Bitboard FILE_B = FILE_A << 1;
constexpr Bitboard FILE_C = FILE_A << 2;
constexpr Bitboard FILE_D = FILE_A << 3;
constexpr Bitboard FILE_E = FILE_A << 4;
constexpr Bitboard FILE_F = FILE_A << 5;
constexpr Bitboard FILE_G = FILE_A << 6;
constexpr Bitboard FILE_H = FILE_A << 7;

// Rank constants (rows 1-8)
constexpr Bitboard RANK_1 = 0xFFULL;
constexpr Bitboard RANK_2 = RANK_1 << 8;
constexpr Bitboard RANK_3 = RANK_1 << 16;
constexpr Bitboard RANK_4 = RANK_1 << 24;
constexpr Bitboard RANK_5 = RANK_1 << 32;
constexpr Bitboard RANK_6 = RANK_1 << 40;
constexpr Bitboard RANK_7 = RANK_1 << 48;
constexpr Bitboard RANK_8 = RANK_1 << 56;

// File and rank arrays for easy indexing
constexpr Bitboard FILES[8] = { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
constexpr Bitboard RANKS[8] = { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };

// Direction offsets for sliding pieces
constexpr int NORTH = 8;
constexpr int SOUTH = -8;
constexpr int EAST = 1;
constexpr int WEST = -1;
constexpr int NORTH_EAST = 9;
constexpr int NORTH_WEST = 7;
constexpr int SOUTH_EAST = -7;
constexpr int SOUTH_WEST = -9;

// Helper functions

// Get file (0-7) from square
constexpr int fileOf(Square sq) {
    return sq & 7;
}

// Get rank (0-7) from square
constexpr int rankOf(Square sq) {
    return sq >> 3;
}

// Create square from file and rank
constexpr Square makeSquare(int file, int rank) {
    return rank * 8 + file;
}

// Get bitboard with single bit set for square
constexpr Bitboard squareBB(Square sq) {
    return 1ULL << sq;
}

// Check if square is valid
constexpr bool isValidSquare(Square sq) {
    return sq >= 0 && sq < 64;
}

// Opposite color
constexpr Color operator~(Color c) {
    return static_cast<Color>(c ^ 1);
}

// Castling rights operators
constexpr CastlingRights operator|(CastlingRights a, CastlingRights b) {
    return static_cast<CastlingRights>(static_cast<int>(a) | static_cast<int>(b));
}

constexpr CastlingRights operator&(CastlingRights a, CastlingRights b) {
    return static_cast<CastlingRights>(static_cast<int>(a) & static_cast<int>(b));
}

constexpr CastlingRights& operator|=(CastlingRights& a, CastlingRights b) {
    return a = a | b;
}

constexpr CastlingRights& operator&=(CastlingRights& a, CastlingRights b) {
    return a = a & b;
}

constexpr CastlingRights operator~(CastlingRights cr) {
    return static_cast<CastlingRights>(~static_cast<int>(cr));
}

// Convert coordinate string (e.g., "e4") to square index
inline Square coordinateToSquare(const std::string& coord) {
    if (coord.length() < 2) return NO_SQUARE;
    int file = coord[0] - 'a';
    int rank = coord[1] - '1';
    if (file < 0 || file > 7 || rank < 0 || rank > 7) return NO_SQUARE;
    return makeSquare(file, rank);
}

// Convert square index to coordinate string
inline std::string squareToCoordinate(Square sq) {
    if (sq == NO_SQUARE || sq < 0 || sq >= 64) return "";
    return std::string(1, 'a' + fileOf(sq)) + std::string(1, '1' + rankOf(sq));
}

// Move encoding helpers

// Create a normal move
constexpr Move makeMove(Square from, Square to) {
    return static_cast<Move>(from | (to << 6));
}

// Create a move with type and optional promotion piece
constexpr Move makeMove(Square from, Square to, MoveType type, PieceType promo = KNIGHT) {
    return static_cast<Move>(from | (to << 6) | ((promo - KNIGHT) << 12) | (type << 14));
}

// Extract from-square from move
constexpr Square moveFrom(Move m) {
    return m & 0x3F;
}

// Extract to-square from move
constexpr Square moveTo(Move m) {
    return (m >> 6) & 0x3F;
}

// Extract move type
constexpr MoveType moveType(Move m) {
    return static_cast<MoveType>((m >> 14) & 0x3);
}

// Extract promotion piece type (only valid if moveType == PROMOTION)
constexpr PieceType movePromotion(Move m) {
    return static_cast<PieceType>(((m >> 12) & 0x3) + KNIGHT);
}

// Check if move is a capture (requires position context, so just a declaration)
// This will be implemented in Position class

// Piece character representation
inline char pieceToChar(PieceType pt, Color c) {
    constexpr char pieces[] = "pnbrqk";
    char ch = pieces[pt];
    return c == WHITE ? static_cast<char>(ch - 32) : ch;  // Uppercase for white
}

// Parse piece from character
inline std::pair<PieceType, Color> charToPiece(char c) {
    Color color = (c >= 'A' && c <= 'Z') ? WHITE : BLACK;
    char lower = (c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c;

    switch (lower) {
        case 'p': return {PAWN, color};
        case 'n': return {KNIGHT, color};
        case 'b': return {BISHOP, color};
        case 'r': return {ROOK, color};
        case 'q': return {QUEEN, color};
        case 'k': return {KING, color};
        default:  return {NO_PIECE_TYPE, NO_COLOR};
    }
}

// Convert move to UCI string (e.g., "e2e4", "e7e8q")
inline std::string moveToUCI(Move m) {
    if (m == NO_MOVE) return "0000";

    std::string result = squareToCoordinate(moveFrom(m)) + squareToCoordinate(moveTo(m));

    if (moveType(m) == PROMOTION) {
        constexpr char promoChars[] = "nbrq";
        result += promoChars[movePromotion(m) - KNIGHT];
    }

    return result;
}

} // namespace Bitboard

#endif // BITBOARD_TYPES_H
