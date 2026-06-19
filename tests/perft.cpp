#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <string>
#include "../headers/bitboard/BitboardEngine.h"
#include "../headers/bitboard/MagicBitboards.h"
#include "../headers/bitboard/Zobrist.h"
#include "../headers/bitboard/MoveGenerator.h"

using namespace Bitboard;

struct PerftTest {
    std::string name;
    std::string fen;
    std::vector<uint64_t> expectedNodes;  // Expected node counts for depths 1, 2, 3, ...
};

// Standard perft test positions
std::vector<PerftTest> perftTests = {
    {
        "Starting Position",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        {20, 400, 8902, 197281, 4865609}  // Depth 1-5
    },
    {
        "Kiwipete",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        {48, 2039, 97862, 4085603}  // Depth 1-4
    },
    {
        "Position 3",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
        {14, 191, 2812, 43238, 674624}  // Depth 1-5
    },
    {
        "Position 4",
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        {6, 264, 9467, 422333}  // Depth 1-4
    },
    {
        "Position 5",
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
        {44, 1486, 62379, 2103487}  // Depth 1-4
    },
    {
        "Position 6",
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
        {46, 2079, 89890, 3894594}  // Depth 1-4
    }
};

void runPerftDivide(Position& pos, int depth) {
    std::cout << "\nPerft divide at depth " << depth << ":\n";
    std::cout << std::string(40, '-') << "\n";

    MoveList moves;
    MoveGenerator::generateLegalMoves(pos, moves);

    uint64_t totalNodes = 0;
    StateInfo st;

    for (Move m : moves) {
        pos.makeMove(m, st);
        uint64_t nodes = MoveGenerator::perft(pos, depth - 1);
        pos.unmakeMove(m);

        std::cout << moveToUCI(m) << ": " << nodes << "\n";
        totalNodes += nodes;
    }

    std::cout << std::string(40, '-') << "\n";
    std::cout << "Total: " << totalNodes << "\n";
}

bool runPerftTest(const PerftTest& test, int maxDepth = -1) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Testing: " << test.name << "\n";
    std::cout << "FEN: " << test.fen << "\n";
    std::cout << std::string(60, '=') << "\n";

    Position pos;
    pos.setFromFEN(test.fen);

    bool allPassed = true;
    int testDepth = maxDepth > 0 ? std::min(maxDepth, static_cast<int>(test.expectedNodes.size()))
                                  : static_cast<int>(test.expectedNodes.size());

    for (int depth = 1; depth <= testDepth; ++depth) {
        auto start = std::chrono::high_resolution_clock::now();
        uint64_t nodes = MoveGenerator::perft(pos, depth);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        double nps = duration.count() > 0 ? (nodes * 1000.0 / duration.count()) : 0;

        uint64_t expected = test.expectedNodes[depth - 1];
        bool passed = (nodes == expected);

        std::cout << "Depth " << depth << ": " << std::setw(12) << nodes;
        std::cout << " (expected " << std::setw(12) << expected << ") ";
        std::cout << std::setw(6) << duration.count() << "ms ";
        std::cout << std::setw(10) << static_cast<uint64_t>(nps) << " nps ";
        std::cout << (passed ? "[PASS]" : "[FAIL]") << "\n";

        if (!passed) {
            allPassed = false;
            // Show divide for debugging
            if (depth <= 3) {
                runPerftDivide(pos, depth);
            }
        }
    }

    return allPassed;
}

void runAllTests(int maxDepth = 4) {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "     PERFT TEST SUITE\n";
    std::cout << "========================================\n";

    int passed = 0;
    int failed = 0;

    for (const auto& test : perftTests) {
        if (runPerftTest(test, maxDepth)) {
            ++passed;
        } else {
            ++failed;
        }
    }

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "     RESULTS\n";
    std::cout << "========================================\n";
    std::cout << "Passed: " << passed << "/" << (passed + failed) << "\n";
    std::cout << "Failed: " << failed << "/" << (passed + failed) << "\n";
    std::cout << "========================================\n";
}

void runBenchmark() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "     PERFORMANCE BENCHMARK\n";
    std::cout << "========================================\n";

    Position pos;
    pos.reset();

    std::cout << "\nStarting position perft benchmark:\n";

    for (int depth = 1; depth <= 6; ++depth) {
        auto start = std::chrono::high_resolution_clock::now();
        uint64_t nodes = MoveGenerator::perft(pos, depth);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        double nps = duration.count() > 0 ? (nodes * 1000.0 / duration.count()) : 0;

        std::cout << "Depth " << depth << ": ";
        std::cout << std::setw(12) << nodes << " nodes, ";
        std::cout << std::setw(8) << duration.count() << " ms, ";
        std::cout << std::setw(12) << static_cast<uint64_t>(nps) << " nps\n";

        // Stop if taking too long
        if (duration.count() > 30000) {
            std::cout << "(stopping - taking too long)\n";
            break;
        }
    }
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [command] [options]\n";
    std::cout << "\nCommands:\n";
    std::cout << "  test [depth]    Run perft tests (default depth 4)\n";
    std::cout << "  benchmark       Run performance benchmark\n";
    std::cout << "  perft <fen> <depth>   Run perft on custom position\n";
    std::cout << "  divide <fen> <depth>  Run perft divide on custom position\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " test\n";
    std::cout << "  " << programName << " test 5\n";
    std::cout << "  " << programName << " benchmark\n";
    std::cout << "  " << programName << " perft \"startpos\" 6\n";
    std::cout << "  " << programName << " divide \"startpos\" 3\n";
}

int main(int argc, char* argv[]) {
    // Initialize the engine
    std::cout << "Initializing magic bitboards...\n";
    Zobrist::initialize();
    MagicBitboards::initialize();
    std::cout << "Initialization complete.\n";

    if (argc < 2) {
        runAllTests(4);
        return 0;
    }

    std::string command = argv[1];

    if (command == "test") {
        int depth = 4;
        if (argc >= 3) {
            depth = std::stoi(argv[2]);
        }
        runAllTests(depth);
    }
    else if (command == "benchmark") {
        runBenchmark();
    }
    else if (command == "perft" && argc >= 4) {
        std::string fen = argv[2];
        int depth = std::stoi(argv[3]);

        if (fen == "startpos") {
            fen = START_FEN;
        }

        Position pos;
        pos.setFromFEN(fen);

        std::cout << "FEN: " << pos.toFEN() << "\n";
        pos.print();

        auto start = std::chrono::high_resolution_clock::now();
        uint64_t nodes = MoveGenerator::perft(pos, depth);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        double nps = duration.count() > 0 ? (nodes * 1000.0 / duration.count()) : 0;

        std::cout << "\nPerft(" << depth << ") = " << nodes << "\n";
        std::cout << "Time: " << duration.count() << " ms\n";
        std::cout << "Speed: " << static_cast<uint64_t>(nps) << " nps\n";
    }
    else if (command == "divide" && argc >= 4) {
        std::string fen = argv[2];
        int depth = std::stoi(argv[3]);

        if (fen == "startpos") {
            fen = START_FEN;
        }

        Position pos;
        pos.setFromFEN(fen);

        std::cout << "FEN: " << pos.toFEN() << "\n";
        pos.print();

        runPerftDivide(pos, depth);
    }
    else if (command == "help" || command == "-h" || command == "--help") {
        printUsage(argv[0]);
    }
    else {
        std::cerr << "Unknown command: " << command << "\n";
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}
