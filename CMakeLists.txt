cmake_minimum_required(VERSION 3.22)
project(chess)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Original headers
set(HEADERS
    "headers/Board.h"
    "headers/Game.h"
    "headers/MTexture.h"
    "headers/Piece.h"
    "headers/SharedData.h"
    "headers/Square.h"
    "headers/ChessColored.h"
    "headers/HelperFunctions.h"
    "headers/King.h"
    "headers/Queen.h"
    "headers/Bishop.h"
    "headers/Knight.h"
    "headers/Rook.h"
    "headers/Pawn.h"
    "headers/FDirector.h"
    "headers/QRBPiece.h"
    "headers/MoveLogger.h"
    "headers/PGNGameDetails.h"
    "headers/Env.h"
)

# Original sources
set(SOURCES
    "sources/Game.cpp"
    "sources/MTexture.cpp"
    "sources/ChessColored.cpp"
    "sources/Board.cpp"
    "sources/Piece.cpp"
    "sources/Rook.cpp"
    "sources/Pawn.cpp"
    "sources/Queen.cpp"
    "sources/King.cpp"
    "sources/Bishop.cpp"
    "sources/Knight.cpp"
    "sources/HelperFunctions.cpp"
    "sources/FDirector.cpp"
    "sources/Square.cpp"
    "sources/QRBPiece.cpp"
    "sources/MoveLogger.cpp"
    "sources/PGNGameDetails.cpp"
)

# Bitboard engine headers
set(BITBOARD_HEADERS
    "headers/bitboard/BitboardTypes.h"
    "headers/bitboard/BitboardUtils.h"
    "headers/bitboard/Zobrist.h"
    "headers/bitboard/MagicBitboards.h"
    "headers/bitboard/Position.h"
    "headers/bitboard/MoveGenerator.h"
    "headers/bitboard/BitboardEngine.h"
    "headers/BitboardBridge.h"
)

# Bitboard engine sources (core - no OOP dependencies)
set(BITBOARD_CORE_SOURCES
    "sources/bitboard/Zobrist.cpp"
    "sources/bitboard/MagicBitboards.cpp"
    "sources/bitboard/Position.cpp"
    "sources/bitboard/MoveGenerator.cpp"
    "sources/bitboard/BitboardEngine.cpp"
)

# Bitboard bridge (requires OOP classes)
set(BITBOARD_BRIDGE_SOURCES
    "sources/BitboardBridge.cpp"
)

# Combined bitboard sources for main application
set(BITBOARD_SOURCES
    ${BITBOARD_CORE_SOURCES}
    ${BITBOARD_BRIDGE_SOURCES}
)

find_package(SDL2 REQUIRED SDL2)

# Main chess application
add_executable(chess.out main.cpp ${HEADERS} ${SOURCES} ${BITBOARD_HEADERS} ${BITBOARD_SOURCES})
target_link_libraries(chess.out PRIVATE SDL2 SDL2_image SDL2_ttf)

# PGN tester
add_executable(tester.out tester.cpp ${HEADERS} ${SOURCES} ${BITBOARD_HEADERS} ${BITBOARD_SOURCES})
target_link_libraries(tester.out PRIVATE SDL2 SDL2_image SDL2_ttf)

# Perft test (standalone, no SDL or OOP dependency)
add_executable(perft.out tests/perft.cpp ${BITBOARD_HEADERS} ${BITBOARD_CORE_SOURCES})
