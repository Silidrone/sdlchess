#include <sstream>
#include <fstream>
#include <cctype>
#include "../headers/HelperFunctions.h"
#include "../headers/Queen.h"
#include "../headers/Square.h"
#include "../headers/Rook.h"
#include "../headers/Bishop.h"
#include "../headers/Knight.h"

std::string
HelperFunctions::get_algebraic_notation(char piece_letter, const std::string &current_coordinate, bool captured) {
    if (captured) {
        return std::string() + piece_letter + 'x' + current_coordinate;
    } else {
        return piece_letter + current_coordinate;
    }
}

Piece *HelperFunctions::getChosenPromotedPieceWithModal(ChessColor color, SDL_Rect square_rect, Board *board) {
    auto renderer = SharedData::instance().getRenderer();
    SDL_Event e;
    std::string path_prefix = std::string("../resources/");
    std::string piece_name_prefix = (color == ChessColor::WHITE ? "w_" : "b_");
    SDL_Rect modal_rect = {square_rect.x, square_rect.y, square_rect.w, square_rect.h * 4};
    SDL_Rect cancel_sign_rect = {modal_rect.x + modal_rect.w - modal_rect.w / 8 - 1, modal_rect.y + 1,
                                 modal_rect.w / 8, modal_rect.h / 20};
    SDL_Rect queen_rect = {modal_rect.x + 15, modal_rect.y + 10, modal_rect.w - 30, modal_rect.h / 4 - 20};
    SDL_Rect rook_rect = {modal_rect.x + 15, modal_rect.y + modal_rect.h / 4 + 10, modal_rect.w - 30,
                          modal_rect.h / 4 - 20};
    SDL_Rect bishop_rect = {modal_rect.x + 15, modal_rect.y + 2 * modal_rect.h / 4 + 10, modal_rect.w - 30,
                            modal_rect.h / 4 - 20};
    SDL_Rect knight_rect = {modal_rect.x + 15, modal_rect.y + 3 * modal_rect.h / 4 + 10, modal_rect.w - 30,
                            modal_rect.h / 4 - 20};

    MTexture queen_texture(renderer, path_prefix + piece_name_prefix + "queen.png");
    MTexture rook_texture(renderer, path_prefix + piece_name_prefix + "rook.png");
    MTexture bishop_texture(renderer, path_prefix + piece_name_prefix + "bishop.png");
    MTexture knight_texture(renderer, path_prefix + piece_name_prefix + "knight.png");
    MTexture cancel_sign_texture(renderer, path_prefix + "cancel_sign.png");
    while (true) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                SDL_Point point({mouse_x, mouse_y});
                if (SDL_PointInRect(&point, &cancel_sign_rect) == SDL_TRUE) {
                    return nullptr;
                }
                if (SDL_PointInRect(&point, &queen_rect) == SDL_TRUE) {
                    return new Queen(color, board);
                }
                if (SDL_PointInRect(&point, &rook_rect) == SDL_TRUE) {
                    return new Rook(color, board);
                }
                if (SDL_PointInRect(&point, &bishop_rect) == SDL_TRUE) {
                    return new Bishop(color, board);
                }
                if (SDL_PointInRect(&point, &knight_rect) == SDL_TRUE) {
                    return new Knight(color, board);
                }
            }
        }


        SDL_RenderFillRect(renderer, &modal_rect);
        queen_texture.render(queen_rect);
        rook_texture.render(rook_rect);
        bishop_texture.render(bishop_rect);
        knight_texture.render(knight_rect);
        cancel_sign_texture.render(cancel_sign_rect);
        //Update screen
        SDL_RenderPresent(renderer);
    }
}

const char *ws = " \t\n\r\f\v";

std::string &HelperFunctions::rtrim(std::string &s) {
    s.erase(s.find_last_not_of(ws) + 1);
    return s;
}

std::string &HelperFunctions::ltrim(std::string &s) {
    s.erase(0, s.find_first_not_of(ws));
    return s;
}

std::string &HelperFunctions::trim(std::string &s) {
    return HelperFunctions::ltrim(HelperFunctions::rtrim(s));
}

std::vector<std::string> HelperFunctions::split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

Piece *HelperFunctions::matchingPiece(const std::string &target, std::vector<Piece *> &possible_pieces,
                                      char column, char row) {
    for (auto &possible_piece: possible_pieces) {
        const auto possible_piece_coordinate = possible_piece->getSquare()->getCoordinate();
        if (column && possible_piece_coordinate.at(0) != column) continue;
        if (row && possible_piece_coordinate.at(1) != row) continue;

        auto attacked_squares = possible_piece->attacked_squares();
        for (auto &moveable_square: possible_piece->moveable_squares(attacked_squares)) {
            if (moveable_square->getCoordinate() == target && possible_piece->move(moveable_square, true)) {
                return possible_piece;
            }
        }
    }

    return nullptr;
}

std::vector<PGNGameDetails> HelperFunctions::parsePGN(std::string &&file_path) {
    std::ifstream file;
    file.open(std::move(file_path));

    std::vector<PGNGameDetails> games;

    bool prev_header = true;
    std::string event, site, date, round, white_name, black_name, result, whiteElo, blackElo, eco, time_control, termination;
    std::vector<std::string> moves;
    auto reset_to_default = [&]() {
        games.push_back(
                PGNGameDetails{moves, event, site, date, round, white_name, black_name, result, whiteElo,
                               blackElo, eco, time_control, termination});
        event = site = date = round = white_name = black_name = result = whiteElo = blackElo = eco = time_control = termination = "";
        moves.clear();
    };
    while (file.good()) {
        std::string line;
        std::getline(file, line);
        if (line.empty()) {
            reset_to_default();
            break;
        };
        char first_char = line.at(0);
        if (isspace(first_char)) continue;
        else {
            if (first_char == '[') { //parse game info
                if (!prev_header) { //if it's a new game to be parsed, set game info values and moves back to default
                    reset_to_default();
                }
                prev_header = true;
                auto separator_pos = line.find_first_of(' ');
                std::string header_key = line.substr(1, separator_pos - 1);
                int a = line.find_last_of('"');
                std::string header_value = line.substr(separator_pos + 2, line.find_last_of('"') - (separator_pos + 2));
                if (header_key == "Event") {
                    event = header_value;
                } else if (header_key == "Site") {
                    site = header_value;
                } else if (header_key == "Date") {
                    date = header_value;
                } else if (header_key == "Round") {
                    round = header_value;
                } else if (header_key == "White") {
                    white_name = header_value;
                } else if (header_key == "Black") {
                    black_name = header_value;
                } else if (header_key == "Result") {
                    result = header_value;
                } else if (header_key == "WhiteElo") {
                    whiteElo = header_value;
                } else if (header_key == "BlackElo") {
                    blackElo = header_value;
                } else if (header_key == "ECO") {
                    eco = header_value;
                } else if (header_key == "TimeControl") {
                    time_control = header_value;
                } else if (header_key == "Termination") {
                    termination = header_value;
                }
            } else if (std::isdigit(first_char)) { // parse game moves
                prev_header = false;
                auto parsed_moves = HelperFunctions::split(line, ' ');
                for (auto parsed_move: parsed_moves) {
                    if (parsed_move == "") continue;
                    parsed_move = trim(parsed_move);
                    if (parsed_move.find('.') != std::string::npos) {
                        moves.push_back(HelperFunctions::split(parsed_move, '.').at(1));
                    } else if (parsed_move.find('-') != std::string::npos && std::isdigit(parsed_move.at(0))) continue;
                    else {
                        moves.push_back(parsed_move);
                    }
                }
            }
        }
    }
    file.close();

    return games;
}