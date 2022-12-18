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

int main(int argc, char *args[]) {
    SharedData::instance().init();
    std::cout << "tester.cpp: start test" << std::endl;
    auto games = HelperFunctions::parsePGN("../pgn_games/Adams.pgn");
    auto matching_piece = [](std::string move, std::vector<Piece *> &possible_pieces,
                             char piece_specification_char) -> Piece * {
        for (auto &possible_piece: possible_pieces) {
            if (piece_specification_char &&
                possible_piece->getSquare()->getCoordinate()[0] != piece_specification_char)
                continue;
            auto attacked_squares = possible_piece->attacked_squares();
            for (auto &moveable_square: possible_piece->moveable_squares(attacked_squares)) {
                if (moveable_square->getCoordinate() == move) {
                    return possible_piece;
                }
            }
        }

        return nullptr;
    };
    for (int i = 0; i < 1; i++) {
        PGNGameDetails &game = games[i];
        std::cout << "Game " << i + 1 << std::endl;
        MoveLogger moveLogger;
        Board board(moveLogger);
        board.init("../resources/w_square_gray.png", "../resources/b_square_gray.png");
        int k = 1;
        for (int j = 0; j < game.getMoveCount(); j++) {
            auto turn_color = static_cast<ChessColor>(static_cast<bool>(j % 2 == 0));
            auto previous_turn_color = static_cast<ChessColor>(static_cast<bool>(j % 2 != 0));
            std::string move = game.getMove(j);
            if (j % 2 == 0) {
                std::cout << std::endl;
                std::cout << k << ".: ";
                k++;
            }
            std::cout << move << " ";
            Piece *selected_piece = nullptr;
            Square *target_square = nullptr;
            if (move.find('x') != std::string::npos) { // e.g.: Rxb5
                auto capture_move_arr = HelperFunctions::split(move, 'x');
                move = std::string(capture_move_arr[0]);
                target_square = board.get_square_by_coordinate(capture_move_arr[1]);
            }
            if (move.length() == 2 || move.length() == 3 || move.length() == 4) { // e.g.: e4
                std::vector<Piece *> possible_pieces;
                char piece_specification_char = '\0';
                if (move.length() == 2) {
                    for (auto &pawn: board.getPawns(turn_color)) {
                        possible_pieces.push_back(pawn);
                    }
                    target_square = board.get_square_by_coordinate(move);
                } else {
                    char piece_letter = move[0];
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
                    if (move.length() == 3) {
                        move = move.substr(1, move.size() - 1);
                    } else if (move.length() == 4) {
                        piece_specification_char = move[1];
                        move = move.substr(2, move.size() - 2);
                    }
                }

                target_square = board.get_square_by_coordinate(move);
                selected_piece = matching_piece(move, possible_pieces, piece_specification_char);
            } else if (move.find('-') != std::string::npos) { // e.g.: O-O or O-O-O
                selected_piece = board.getKing(turn_color);
                target_square = board.get_square_by_coordinate(move);
            }

            if (selected_piece) {
                if (!selected_piece->move(target_square)) {
                    throw InvalidMove(game.getMove(j), game);
                } else {
                    if (board.isGameOver(turn_color, previous_turn_color)) {
                        if (j != game.getMoveCount() - 1) {
                            throw PrematureCheckmate(game.getMove(j), game);
                        }
                    }
                }
            } else {
                throw InvalidPieceNotation(game.getMove(j), game);
            }
        }
        std::cout << std::endl << std::endl;
    }

    //Destroy window
    SDL_DestroyRenderer(SharedData::instance().getRenderer());
    SDL_DestroyWindow(SharedData::instance().getWindow());

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}