#include <iostream>
#include "headers/HelperFunctions.h"
#include "headers/MoveLogger.h"
#include <exception>
#include "headers/King.h"
#include "headers/Rook.h"
#include "headers/Knight.h"
#include "headers/Bishop.h"
#include "headers/Queen.h"
#include "headers/Pawn.h"
#include <SDL2/SDL.h>

struct ChessGameException : public std::exception {
protected:
    std::string m_move;
    PGNGameDetails m_game;
    std::string m_msg;
public:
    explicit ChessGameException(const PGNGameDetails &game) : m_game(game) {
        m_msg = std::string() + "Event: " + game.getEvent() + "\nSite: " + game.getSite() +
                "\nDate: " + game.getDate() + "\nRound: " + game.getRound() + "\nWhite: " + game.getWhiteName() +
                "\nBlack: " + game.getBlackName() + "\nResult: " + game.getResult() + "\n";
    }

    const char *what() const throw() {
        return m_msg.c_str();
    }
};

struct InvalidMove : ChessGameException {
public:
    InvalidMove(const std::string &move, const PGNGameDetails &game) : ChessGameException(game) {
        m_msg = "Invalid move: " + move + '\n' + m_msg;
    }
};

struct PrematureCheckmate : ChessGameException {
public:
    PrematureCheckmate(const std::string &move, const PGNGameDetails &game) : ChessGameException(game) {
        m_msg = "Premature checkmate: " + move + '\n' + m_msg;
    }
};

struct InvalidPieceNotation : ChessGameException {
    InvalidPieceNotation(const std::string &move, const PGNGameDetails &game) : ChessGameException(game) {
        m_msg = "Invalid piece notation: " + move + '\n' + m_msg;
    }
};

void test_game(const PGNGameDetails &game) {
    MoveLogger moveLogger;
    Board board(moveLogger);
    board.init("../resources/w_square_gray.png", "../resources/b_square_gray.png");

    for (int j = 0; j < game.getMoveCount(); j++) {
        const auto turn_color = static_cast<ChessColor>(static_cast<bool>(j % 2 == 0));
        const auto previous_turn_color = static_cast<ChessColor>(static_cast<bool>(j % 2 != 0));
        std::string move = game.getMove(j);

        Piece *selected_piece = nullptr;
        std::string target_square_coordinate = "";

        const auto check_index = move.find('+');
        if (check_index != std::string::npos) { // remove a check mark if there is one
            move.erase(check_index, 1);
        }

        if (move.find('-') != std::string::npos) { // i.e.: O-O or O-O-O
            selected_piece = board.getKing(turn_color);
            auto fDirector = selected_piece->getFDirector();
            if (move.length() == 3) { // i.e. 0-0 (king-side)
                target_square_coordinate = fDirector.right(
                        fDirector.right(selected_piece->getSquare()->getCoordinate()));
            } else if (move.length() == 5) { // i.e. 0-0-0 (queen-side)
                target_square_coordinate = fDirector.left(
                        fDirector.left(selected_piece->getSquare()->getCoordinate()));
            }
        } else { // e.g. e4, exd4, Bxe4, Bc4, Nbd7
            std::vector<Piece *> possible_pieces;
            char column = '\0';
            char row = '\0';

            if (move.length() == 2) {
                for (auto &pawn: board.getPawns(turn_color)) {
                    possible_pieces.push_back(pawn);
                }
                target_square_coordinate = move;
                column = move.at(0);
            } else {
                char piece_letter = move.at(0);
                if (std::islower(piece_letter)) {
                    for (auto &pawn: board.getPawns(turn_color)) {
                        possible_pieces.push_back(pawn);
                    }
                } else {
                    if (piece_letter == 'K') {
                        possible_pieces.push_back(board.getKing(turn_color));
                    } else if (piece_letter == 'Q') {
                        possible_pieces.push_back(board.getQueen(turn_color));
                    } else if (piece_letter == 'B') {
                        for (auto &bishop: board.getBishops(turn_color)) {
                            possible_pieces.push_back(bishop);
                        }
                    } else if (piece_letter == 'N') {
                        for (auto &knight: board.getKnights(turn_color)) {
                            possible_pieces.push_back(knight);
                        }
                    } else if (piece_letter == 'R') {
                        for (auto &rook: board.getRooks(turn_color)) {
                            possible_pieces.push_back(rook);
                        }
                    }
                }

                if (move.find('x') != std::string::npos) {
                    auto capture_move_arr = HelperFunctions::split(move, 'x');
                    if (capture_move_arr[0].size() == 1) {
                        target_square_coordinate = capture_move_arr[1];
                        const char first_letter = capture_move_arr[0].at(0);
                        if (std::islower(first_letter)) {
                            column = first_letter;
                        }
                    }
                } else if (move.length() == 3) {
                    target_square_coordinate = move.substr(1, move.size() - 1);
                } else if (move.length() == 4) {
                    target_square_coordinate = move.substr(2, move.size() - 2);
                    const char ambiguity_clarifier_letter = move[1];
                    if (std::isdigit(ambiguity_clarifier_letter)) {
                        row = ambiguity_clarifier_letter;
                    } else {
                        column = ambiguity_clarifier_letter;
                    }
                }
            }

            selected_piece = HelperFunctions::matchingPiece(target_square_coordinate, possible_pieces, column, row);
        }

        if (selected_piece && !target_square_coordinate.empty()) {
            if (!selected_piece->move(board.get_square_by_coordinate(target_square_coordinate))) {
                throw InvalidMove(move, game);
            } else {
                if (board.isGameOver(turn_color, previous_turn_color)) {
                    if (j != game.getMoveCount() - 1) {
                        throw PrematureCheckmate(move, game);
                    }
                }
            }
        } else {
            throw InvalidPieceNotation(move, game);
        }
    }
}

int main(int argc, char *args[]) {
    SharedData::instance().init();
    std::cout << "tester.cpp: start test" << std::endl;
    auto games = HelperFunctions::parsePGN("../pgn_games/Adams.pgn");
    unsigned long long succeeded_game_count = 0;
    for (int i = 1095; i < games.size(); i++) {
        PGNGameDetails &game = games[i];
        try {
            test_game(game);
            succeeded_game_count++;
        } catch (ChessGameException &exception) {
            std::cout << "Game No. " << i + 1 << " failed. Reason: " << exception.what() << std::endl;
        }
    }

    std::cout << (float)succeeded_game_count / games.size() * 100 << "% games succeeded" << std::endl;

    //Destroy window
    SDL_DestroyRenderer(SharedData::instance().getRenderer());
    SDL_DestroyWindow(SharedData::instance().getWindow());

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}