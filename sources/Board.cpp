#include "../headers/Board.h"
#include "../headers/SharedData.h"
#include "../headers/Piece.h"
#include "../headers/Square.h"
#include "../headers/King.h"
#include "../headers/Rook.h"
#include "../headers/Knight.h"
#include "../headers/Bishop.h"
#include "../headers/Queen.h"
#include "../headers/Pawn.h"
#include "../headers/MoveLogger.h"
#include "../headers/HelperFunctions.h"
#include <algorithm>
#include <stdexcept>

Board::Board(MoveLogger &moveLogger) : m_squares(), m_pieces(), m_moveLogger(moveLogger) {}

void Board::init(const std::string &w_texture_path, const std::string &b_texture_path) {
    const int square_width = SharedData::instance().SCREEN_WIDTH / ROW_SQUARE_COUNT;
    const int square_height = SharedData::instance().SCREEN_HEIGHT / COLUMN_SQUARE_COUNT;
    const SDL_Color text_color = SDL_Color({255, 255, 255});

    SDL_Renderer *renderer = SharedData::instance().getRenderer();
    MTexture w_square_texture(renderer, w_texture_path);
    MTexture b_square_texture(renderer, b_texture_path);
    bool square_white = static_cast<bool>(ChessColor::BLACK);

    for (int i = 0; i < ROW_SQUARE_COUNT; i++) {
        m_coordinate_textures.at(0).push_back(std::pair<SDL_Rect, MTexture>(
                {5, i * square_height + 5, 25, 25}, MTexture(renderer,
                                                             std::string() +
                                                             static_cast<char>('0' + COLUMN_SQUARE_COUNT - i),
                                                             text_color)));
        for (int j = 0; j < COLUMN_SQUARE_COUNT; j++, square_white = !square_white) {
            if (i == ROW_SQUARE_COUNT - 1) {
                m_coordinate_textures.at(1).push_back(std::pair<SDL_Rect, MTexture>(
                        {j * square_width + (square_width - 30), i * square_height + (square_height - 30), 25, 25},
                        MTexture(renderer, std::string() + static_cast<char>('a' + j), text_color)));
            }

            MTexture square_texture(renderer);
            if (i > 0 && j == 0) {
                square_texture = m_squares.back()->getTexture();
                square_white = !square_white;
            } else {
                square_texture = square_white ? w_square_texture : b_square_texture;
            }

            m_squares.push_back(new Square(std::string() + static_cast<char>(j + 'a') + static_cast<char>('1' + i),
                                           static_cast<ChessColor>(square_white),
                                           square_texture,
                                           SDL_Rect{j * square_width, square_height * (ROW_SQUARE_COUNT - i - 1),
                                                    square_width, square_height}));
        }
    }
    m_pieces = {
            //white pieces
            new Rook(ChessColor::WHITE, this, m_squares[0]),
            new Knight(ChessColor::WHITE, this, m_squares[1]),
            new Bishop(ChessColor::WHITE, this, m_squares[2]),
            new Queen(ChessColor::WHITE, this, m_squares[3]),
            new King(ChessColor::WHITE, this, m_squares[4]),
            new Bishop(ChessColor::WHITE, this, m_squares[5]),
            new Knight(ChessColor::WHITE, this, m_squares[6]),
            new Rook(ChessColor::WHITE, this, m_squares[7]),
            new Pawn(ChessColor::WHITE, this, m_squares[8]),
            new Pawn(ChessColor::WHITE, this, m_squares[9]),
            new Pawn(ChessColor::WHITE, this, m_squares[10]),
            new Pawn(ChessColor::WHITE, this, m_squares[11]),
            new Pawn(ChessColor::WHITE, this, m_squares[12]),
            new Pawn(ChessColor::WHITE, this, m_squares[13]),
            new Pawn(ChessColor::WHITE, this, m_squares[14]),
            new Pawn(ChessColor::WHITE, this, m_squares[15]),

            //black pieces
            new Rook(ChessColor::BLACK, this, m_squares[63]),
            new Knight(ChessColor::BLACK, this, m_squares[62]),
            new Bishop(ChessColor::BLACK, this, m_squares[61]),
            new King(ChessColor::BLACK, this, m_squares[60]),
            new Queen(ChessColor::BLACK, this, m_squares[59]),
            new Bishop(ChessColor::BLACK, this, m_squares[58]),
            new Knight(ChessColor::BLACK, this, m_squares[57]),
            new Rook(ChessColor::BLACK, this, m_squares[56]),
            new Pawn(ChessColor::BLACK, this, m_squares[55]),
            new Pawn(ChessColor::BLACK, this, m_squares[54]),
            new Pawn(ChessColor::BLACK, this, m_squares[53]),
            new Pawn(ChessColor::BLACK, this, m_squares[52]),
            new Pawn(ChessColor::BLACK, this, m_squares[51]),
            new Pawn(ChessColor::BLACK, this, m_squares[50]),
            new Pawn(ChessColor::BLACK, this, m_squares[49]),
            new Pawn(ChessColor::BLACK, this, m_squares[48]),
    };

    updateAttackedSquares();
}

Board::~Board() {
    for (auto &p: m_pieces) {
        delete p;
    }

    for (auto &s: m_squares) {
        delete s;
    }

    for (auto &rp: m_removed_pieces) {
        delete rp;
    }
}

void Board::render() {
    for (auto &square: m_squares) {
        square->render();
    }

    for (auto &piece: m_pieces) {
        piece->render();
    }

    for (auto &crc: m_coordinate_textures) {
        for (auto &dest_texture_pair: crc) {
            dest_texture_pair.second.render(dest_texture_pair.first);
        }
    }

// debug for white/black attack count on each square
//    for (auto &square: m_squares) {
//        auto destination = square->getDestination();
//        MTexture white_attack_count_texture(SharedData::instance().getRenderer(),
//                                            std::string("W: ") +
//                                            static_cast<char>('0' + square->getAttackCount(ChessColor::WHITE)),
//                                            SDL_Color({255, 0, 0}));
//        MTexture black_attack_count_texture(SharedData::instance().getRenderer(),
//                                            std::string("B: ") +
//                                            static_cast<char>('0' + square->getAttackCount(ChessColor::BLACK)),
//                                            SDL_Color({0, 0, 255}));
//        white_attack_count_texture.render({destination.x + destination.w / 2 - 30, destination.y + destination.h / 2,
//                                           35, 35});
//        black_attack_count_texture.render({destination.x + destination.w / 2 + 20, destination.y + destination.h / 2,
//                                           35, 35});
//    }
}

Square *Board::get_square_by_screen_position(int x, int y) {
    SDL_Point point({x, y});
    for (auto &square: m_squares) {
        SDL_Rect square_destination = square->getDestination();
        if (SDL_PointInRect(&point, &square_destination) == SDL_TRUE) {
            return square;
        }
    }

    return nullptr;
}

Square *Board::get_square_by_coordinate(const std::string &coordinate) {
    for (auto &square: m_squares) {
        if (square->getCoordinate() == coordinate) return square;
    }

    return nullptr;
}

bool Board::coordinateIsValid(std::string coordinate) {
    return coordinate[0] >= 'a' + 0 && coordinate[1] >= '1' + 0 && coordinate[0] < 'a' + ROW_SQUARE_COUNT &&
           coordinate[1] < '1' + COLUMN_SQUARE_COUNT;
}

bool Board::legalMoveExists(ChessColor c) {
    std::vector<Piece *> pieces = m_pieces;
    for (auto &piece: pieces) {
        if (piece->getColor() == c) {
            auto attacked_squares = piece->attacked_squares();
            auto moveable_squares = piece->moveable_squares(attacked_squares);
            for (auto &moveable_square: moveable_squares) {
                if (piece->move(moveable_square, true)) {
                    return true;
                }
            }
        }
    }

    return false;
}

std::vector<Square *>
Board::get_squares_in_fdirection(Piece *starting_piece, FDirection fDirection, bool one_step) {
    std::vector<Square *> path_squares;
    std::string coordinate = (starting_piece->getFDirector().*fDirection)(starting_piece->getSquare()->getCoordinate());

    auto square_add = [this, &coordinate, &path_squares]() {
        Square *s = get_square_by_coordinate(coordinate);
        if (s) {
            path_squares.push_back(s);
        }
    };

    if (!one_step) {
        while (coordinateIsValid(coordinate)) {
            square_add();
            coordinate = (starting_piece->getFDirector().*fDirection)(coordinate);
        }
    } else {
        square_add();
    }

    return path_squares;
}

std::pair<std::vector<Square *>, std::vector<DirectionalSquares>>
Board::get_squares_in_fdirections(Piece *starting_piece, std::vector<std::pair<Direction, FDirection>> directions,
                                  bool one_step) {
    std::vector<DirectionalSquares> result;
    std::vector<Square *> all_squares;
    for (auto &direction: directions) {
        auto squares = get_squares_in_fdirection(starting_piece, direction.second, one_step);
        result.push_back({direction.first, squares});
        all_squares.insert(all_squares.end(), squares.begin(), squares.end());
    }

    return {all_squares, result};
}

void Board::updateRenderOrder() {
    std::sort(m_pieces.begin(), m_pieces.end(), [](Piece *&a, Piece *&b) -> bool {
        return a->getRenderPriority() > b->getRenderPriority();
    });
}

void Board::rotate180() {
    for (int i = 0, j = COLUMN_SQUARE_COUNT - 1; i <= ROW_SQUARE_COUNT / 2 - 1 && j >= ROW_SQUARE_COUNT / 2; i++, j--) {
        for (int k = 0; k < ROW_SQUARE_COUNT; k++) {
            Square *a = m_squares[i * ROW_SQUARE_COUNT + k];
            SDL_Rect a_destination = a->getDestination();
            Square *b = m_squares[j * ROW_SQUARE_COUNT + k];
            SDL_Rect b_destination = b->getDestination();
            a->setDestination(b_destination.x, b_destination.y);
            b->setDestination(a_destination.x, a_destination.y);
        }
    }
    auto &row_coordinate_textures = m_coordinate_textures[0];
    for (int i = 0; i < row_coordinate_textures.size() / 2; i++) {
        SDL_Rect tmp_rect = row_coordinate_textures[i].first;
        row_coordinate_textures[i].first = row_coordinate_textures[row_coordinate_textures.size() - i - 1].first;
        row_coordinate_textures[row_coordinate_textures.size() - i - 1].first = tmp_rect;
    }
}

void Board::updateAttackedSquares() {
    for (auto &piece: m_pieces) {
        piece->unattack_squares();
        piece->attack_squares();
    }
}

void Board::removePiece(Piece *p) {
    for (auto it = m_pieces.begin(); it != m_pieces.end(); it++) {
        if ((*it)->getSquare()->getCoordinate() == p->getSquare()->getCoordinate()) {
            m_removed_pieces.push_back(*it);
            it = m_pieces.erase(it);
            --it;
            break;
        }
    }
}

void Board::unRemoveLastPiece() {
    if (m_removed_pieces.empty())
        throw std::logic_error("Trying to unremove the last removed piece when there is none!");
    auto last = std::prev(m_removed_pieces.end());
    m_pieces.push_back(*last);
    m_removed_pieces.erase(last);
}

void Board::addPiece(Piece *p) {
    m_pieces.push_back(p);
}

template<class T>
std::vector<T *> Board::getPieces(ChessColor c) {
    std::vector<T *> result;
    for (auto &piece: m_pieces) {
        T *t_piece = dynamic_cast<T *>(piece);
        if (t_piece != nullptr && t_piece->getColor() == c) result.push_back(t_piece);
    }

    return result;
}

King *Board::getKing(ChessColor c) {
    auto kings = getPieces<King>(c);
    if (kings.size() > 0) return kings[0];
    return nullptr;
}

std::vector<Queen *> Board::getQueens(ChessColor c) {
   return getPieces<Queen>(c);
}

std::vector<Pawn *> Board::getPawns(ChessColor c) {
    return getPieces<Pawn>(c);
}

std::vector<Bishop *> Board::getBishops(ChessColor c) {
    return getPieces<Bishop>(c);
}

std::vector<Knight *> Board::getKnights(ChessColor c) {
    return getPieces<Knight>(c);
}

std::vector<Rook *> Board::getRooks(ChessColor c) {
    return getPieces<Rook>(c);
}

bool Board::isGameOver() {
    auto turn_color = m_moveLogger.getCurrentMoveColor();
    auto previous_turn_color = HelperFunctions::oppositeColor(turn_color);
    auto king = getKing(turn_color);
    auto king_attacked_squares = king->attacked_squares();
    return king->getSquare()->isAttacked(previous_turn_color) &&
           king->moveable_squares(king_attacked_squares).empty() &&
           !legalMoveExists(turn_color);
}

MoveLogger &Board::getMoveLogger() const {
    return m_moveLogger;
}