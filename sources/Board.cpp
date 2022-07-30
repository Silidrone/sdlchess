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

Board::Board(const std::string &w_texture_path, const std::string &b_texture_path)
        : m_squares(), m_pieces() {
    const int square_width = SharedData::instance().SCREEN_WIDTH / ROW_SQUARE_COUNT;
    const int square_height = SharedData::instance().SCREEN_HEIGHT / COLUMN_SQUARE_COUNT;
    const SDL_Color text_color = SDL_Color({255, 255, 255});

    SDL_Renderer *renderer = SharedData::instance().getRenderer();
    MTexture w_square_texture(renderer, w_texture_path);
    MTexture b_square_texture(renderer, b_texture_path);
    bool square_white = static_cast<bool>(ChessColor::BLACK);

    for (int i = 0; i < ROW_SQUARE_COUNT; i++) {
        m_coordinate_textures.push_back(std::pair<SDL_Rect, MTexture>(
                {5, i * square_height + 5, 25, 25}, MTexture(renderer,
                                                             std::string() +
                                                             static_cast<char>('0' + COLUMN_SQUARE_COUNT - i),
                                                             text_color)));
        for (int j = 0; j < COLUMN_SQUARE_COUNT; j++, square_white = !square_white) {
            if (i == ROW_SQUARE_COUNT - 1) {
                m_coordinate_textures.push_back(std::pair<SDL_Rect, MTexture>(
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

            Square *new_square = new Square(std::string() + static_cast<char>(j + 'a') + static_cast<char>('1' + i),
                                            static_cast<ChessColor>(square_white),
                                            square_texture,
                                            SDL_Rect{j * square_width, square_height * (ROW_SQUARE_COUNT - i - 1),
                                                     square_width, square_height});
            m_squares.push_back(new_square);
        }
    }
    m_pieces = {
            //white pieces
            new Rook(ChessColor::WHITE, m_squares[0], this),
            new Knight(ChessColor::WHITE, m_squares[1], this),
            new Bishop(ChessColor::WHITE, m_squares[2], this),
            new Queen(ChessColor::WHITE, m_squares[3], this),
            new King(ChessColor::WHITE, m_squares[4], this),
            new Bishop(ChessColor::WHITE, m_squares[5], this),
            new Knight(ChessColor::WHITE, m_squares[6], this),
            new Rook(ChessColor::WHITE, m_squares[7], this),
            new Pawn(ChessColor::WHITE, m_squares[8], this),
            new Pawn(ChessColor::WHITE, m_squares[9], this),
            new Pawn(ChessColor::WHITE, m_squares[10], this),
            new Pawn(ChessColor::WHITE, m_squares[11], this),
            new Pawn(ChessColor::WHITE, m_squares[12], this),
            new Pawn(ChessColor::WHITE, m_squares[13], this),
            new Pawn(ChessColor::WHITE, m_squares[14], this),
            new Pawn(ChessColor::WHITE, m_squares[15], this),

            //black pieces
            new Rook(ChessColor::BLACK, m_squares[63], this),
            new Knight(ChessColor::BLACK, m_squares[62], this),
            new Bishop(ChessColor::BLACK, m_squares[61], this),
            new King(ChessColor::BLACK, m_squares[60], this),
            new Queen(ChessColor::BLACK, m_squares[59], this),
            new Bishop(ChessColor::BLACK, m_squares[58], this),
            new Knight(ChessColor::BLACK, m_squares[57], this),
            new Rook(ChessColor::BLACK, m_squares[56], this),
            new Pawn(ChessColor::BLACK, m_squares[55], this),
            new Pawn(ChessColor::BLACK, m_squares[54], this),
            new Pawn(ChessColor::BLACK, m_squares[53], this),
            new Pawn(ChessColor::BLACK, m_squares[52], this),
            new Pawn(ChessColor::BLACK, m_squares[51], this),
            new Pawn(ChessColor::BLACK, m_squares[50], this),
            new Pawn(ChessColor::BLACK, m_squares[49], this),
            new Pawn(ChessColor::BLACK, m_squares[48], this),
    };
}


void Board::render() {
    for (auto &square: m_squares) {
        square->render();
    }

    for (auto &piece: m_pieces) {
        piece->render();
    }

    for (auto &pair: m_coordinate_textures) {
        pair.second.render(pair.first);
    }
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

std::vector<Square *>
Board::get_squares_in_fdirection(Square *starting_square, FDirection fDirection, bool one_step) {
    std::vector<Square *> path_squares;
    std::string coordinate = fDirection(starting_square->getCoordinate());

    auto square_add = [this, &coordinate, &path_squares]() {
        Square *s = get_square_by_coordinate(coordinate);
        if (s) {
            path_squares.push_back(s);
        }
    };

    if (!one_step) {
        while (coordinateIsValid(coordinate)) {
            square_add();
            coordinate = fDirection(coordinate);
        }
    } else {
        square_add();
    }

    return path_squares;
}

std::pair<std::vector<Square *>, std::vector<DirectionalSquares>>
Board::get_squares_in_fdirections(Square *beginning_square, std::vector<std::pair<Direction, FDirection>> directions,
                                  bool one_step) {
    std::vector<DirectionalSquares> result;
    std::vector<Square *> all_squares;
    for (auto &direction: directions) {
        auto squares = get_squares_in_fdirection(beginning_square, direction.second, one_step);
        result.push_back({direction.first, squares});
        all_squares.insert(all_squares.begin(), squares.begin(), squares.end());
    }

    return {all_squares, result};
}

std::vector<Square *> Board::get_squares_by_direction(std::vector<DirectionalSquares> &legal_squares, Direction d) {
    for (auto &legal_square: legal_squares) {
        if (legal_square.first == d) {
            return legal_square.second;
        }
    }

    return std::vector<Square *>();
}

void Board::swapSquares(Square *&a, Square *&b) {
    Square *c = a;
    a = b;
    b = c;
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

            swapSquares(m_squares[i * ROW_SQUARE_COUNT + k], m_squares[j * ROW_SQUARE_COUNT + k]);
        }
    }
}