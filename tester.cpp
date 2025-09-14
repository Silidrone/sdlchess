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
#include <filesystem>
#include <vector>
#include <string>

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
    InvalidMove(const std::string &move, int move_number, const PGNGameDetails &game) : ChessGameException(game) {
        m_msg = "Invalid move: " + move + "(j=" + std::to_string(move_number) + ")\n" + m_msg;
    }
};

struct PrematureCheckmate : ChessGameException {
public:
    PrematureCheckmate(const std::string &move, int move_number, const PGNGameDetails &game) : ChessGameException(game) {
        m_msg = "Premature checkmate: " + move + "(j=" + std::to_string(move_number) + ")\n" + m_msg;
    }
};

struct InvalidPieceNotation : ChessGameException {
    InvalidPieceNotation(const std::string &move, int move_number, const PGNGameDetails &game) : ChessGameException(game) {
        m_msg = "Invalid piece notation: " + move + "(j=" + std::to_string(move_number) + ")\n" + m_msg;
    }
};

Piece *promotion_method(Pawn *selected_pawn, Board *board, std::string move) {
    if(selected_pawn && move.find('=') != std::string::npos) {
        char promoted_piece_letter = move.back();
        if(promoted_piece_letter == 'Q') {
            return new Queen(selected_pawn->getColor(), board);
        } else if(promoted_piece_letter == 'B') {
            return new Bishop(selected_pawn->getColor(), board);
        } else if(promoted_piece_letter == 'N') {
            return new Knight(selected_pawn->getColor(), board);
        } else if(promoted_piece_letter == 'R') {
            return new Rook(selected_pawn->getColor(), board);
        }
    }

    return nullptr;
}

void test_game(const PGNGameDetails &game) {
    MoveLogger moveLogger;
    Board board(moveLogger);
    auto& resources_path = SharedData::instance().getResourcesPath();
    board.init((resources_path + "w_square_gray.png").c_str(), (resources_path + "b_square_gray.png").c_str());

    for (int j = 0; j < game.getMoveCount(); j++) {
        const auto turn_color = moveLogger.getCurrentMoveColor();
        const auto previous_turn_color = HelperFunctions::oppositeColor(turn_color);
        std::string move = game.getMove(j);

        Piece *selected_piece = nullptr;
        Piece *promoted_piece = nullptr;
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
                        for (auto &queen: board.getQueens(turn_color)) {
                            possible_pieces.push_back(queen);
                        }
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
                
                if(move.find('=') != std::string::npos) {
                    column = move[0];
                    if (move.find('x') != std::string::npos) {
                        target_square_coordinate = HelperFunctions::split(HelperFunctions::split(move, '=')[0], 'x')[1];
                    } else {
                        target_square_coordinate = move.substr(0, 2);
                    }
                    if(move[1] == '8') {
                        row = '7';
                    } else if (move[1] == '1') {
                        row = '2';
                    }
                } else if (move.find('x') != std::string::npos) {
                    auto capture_move_arr = HelperFunctions::split(move, 'x');
                    target_square_coordinate = capture_move_arr.at(1);
                    if (capture_move_arr.at(0).size() == 1) {
                        const char first_letter = capture_move_arr.at(0).at(0);
                        if (std::islower(first_letter)) {
                            column = first_letter;
                        }
                    } else if (capture_move_arr.at(0).size() == 2) {
                        char ambiguity_clarifier_letter = capture_move_arr.at(0).at(1);
                        if (std::isdigit(ambiguity_clarifier_letter)) {
                            row = ambiguity_clarifier_letter;
                        } else {
                            column = ambiguity_clarifier_letter;
                        }
                    }
                } else if (move.length() == 3) {
                    target_square_coordinate = move.substr(1, move.size() - 1);
                } else if (move.length() == 4) {
                    target_square_coordinate = move.substr(2, move.size() - 2);
                    const char ambiguity_clarifier_letter = move.at(1);
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
            if (!selected_piece->move(board.get_square_by_coordinate(target_square_coordinate), false, [&board, &move](Pawn *selected_pawn) {return promotion_method(selected_pawn, &board, move); })) {
                throw InvalidMove(move, j, game);
            } else {
                if (board.isGameOver()) {
                    if (j != game.getMoveCount() - 1) {
                        throw PrematureCheckmate(move, j, game);
                    }
                }
            }
        } else {
            throw InvalidPieceNotation(move, j, game);
        }
    }
}

int main(int argc, char *args[]) {
    SharedData::instance().init();
    std::cout << "tester.cpp: start test" << std::endl;

    auto& resources_path = SharedData::instance().getResourcesPath();
    std::string pgn_directory = resources_path + "pgn_games/";

    std::vector<std::string> pgn_files;
    for (const auto& entry : std::filesystem::directory_iterator(pgn_directory)) {
        if (entry.path().extension() == ".pgn") {
            pgn_files.push_back(entry.path().filename().string());
        }
    }

    unsigned long long total_succeeded_game_count = 0;
    unsigned long long total_game_count = 0;

    for (const auto& pgn_file : pgn_files) {
        std::cout << "Testing " << pgn_file << "..." << std::endl;
        auto games = HelperFunctions::parsePGN(pgn_directory + pgn_file);
        unsigned long long succeeded_game_count = 0;

        for (int i = 0; i < games.size(); i++) {
            PGNGameDetails &game = games[i];
            try {
                test_game(game);
                succeeded_game_count++;
            } catch (ChessGameException &exception) {
                std::cout << "Game No. " << i + 1 << " from " << pgn_file << " failed. Reason: " << exception.what() << std::endl;
            }
        }

        total_succeeded_game_count += succeeded_game_count;
        total_game_count += games.size();

        std::cout << pgn_file << ": " << (float) succeeded_game_count / games.size() * 100 << "% games succeeded ("
                  << succeeded_game_count << "/" << games.size() << ")" << std::endl;
    }

    std::cout << "Overall: " << (float) total_succeeded_game_count / total_game_count * 100 << "% games succeeded ("
              << total_succeeded_game_count << "/" << total_game_count << ")" << std::endl;

    //Destroy window
    SDL_DestroyRenderer(SharedData::instance().getRenderer());
    SDL_DestroyWindow(SharedData::instance().getWindow());

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}