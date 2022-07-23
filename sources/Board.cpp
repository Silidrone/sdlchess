#include "../headers/Board.h"

Board::Board(const std::string &w_texture_path, const std::string &b_texture_path) : m_squares(), m_pieces() {
    const int square_width = SharedData::instance().SCREEN_WIDTH / ROW_SQUARE_COUNT;
    const int square_height = SharedData::instance().SCREEN_HEIGHT / COLUMN_SQUARE_COUNT;
    SDL_Renderer *renderer = SharedData::instance().getRenderer();
    MTexture w_square_texture(renderer, w_texture_path);
    MTexture b_square_texture(renderer, b_texture_path);

    bool square_white = static_cast<bool>(ChessColor::WHITE);
    const SDL_Color text_color = SDL_Color({255, 255, 255});
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

            m_squares.push_back(
                    new Square(convert_indices_to_coordinate(i, j),
                               static_cast<ChessColor>(square_white), square_texture,
                               SDL_Rect{j * square_width, i * square_height, square_width, square_height}));
        }
    }

    m_pieces = {
            //black pieces
            new Rook(ChessColor::BLACK, m_squares[0], this),
            new Knight(ChessColor::BLACK, m_squares[1], this),
            new Bishop(ChessColor::BLACK, m_squares[2], this),
            new Queen(ChessColor::BLACK, m_squares[3], this),
            new King(ChessColor::BLACK, m_squares[4], this),
            new Bishop(ChessColor::BLACK, m_squares[5], this),
            new Knight(ChessColor::BLACK, m_squares[6], this),
            new Rook(ChessColor::BLACK, m_squares[7], this),
            new Pawn(ChessColor::BLACK, m_squares[8], this),
            new Pawn(ChessColor::BLACK, m_squares[9], this),
            new Pawn(ChessColor::BLACK, m_squares[10], this),
            new Pawn(ChessColor::BLACK, m_squares[11], this),
            new Pawn(ChessColor::BLACK, m_squares[12], this),
            new Pawn(ChessColor::BLACK, m_squares[13], this),
            new Pawn(ChessColor::BLACK, m_squares[14], this),
            new Pawn(ChessColor::BLACK, m_squares[15], this),

            //white pieces
            new Rook(ChessColor::WHITE, m_squares[63], this),
            new Knight(ChessColor::WHITE, m_squares[62], this),
            new Bishop(ChessColor::WHITE, m_squares[61], this),
            new King(ChessColor::WHITE, m_squares[60], this),
            new Queen(ChessColor::WHITE, m_squares[59], this),
            new Bishop(ChessColor::WHITE, m_squares[58], this),
            new Knight(ChessColor::WHITE, m_squares[57], this),
            new Rook(ChessColor::WHITE, m_squares[56], this),
            new Pawn(ChessColor::WHITE, m_squares[55], this),
            new Pawn(ChessColor::WHITE, m_squares[54], this),
            new Pawn(ChessColor::WHITE, m_squares[53], this),
            new Pawn(ChessColor::WHITE, m_squares[52], this),
            new Pawn(ChessColor::WHITE, m_squares[51], this),
            new Pawn(ChessColor::WHITE, m_squares[50], this),
            new Pawn(ChessColor::WHITE, m_squares[49], this),
            new Pawn(ChessColor::WHITE, m_squares[48], this),
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

Square *Board::get_square_by_coordinate(std::string coordinate) {
    return get_square_by_indices_pair(convert_coordinate_to_indices(coordinate));
}

Square *Board::get_square_by_indices_pair(std::pair<int, int> indices) {
    return get_square_by_indices(indices.first, indices.second);
}

Square *Board::get_square_by_indices(int i, int j) {
    return m_squares.at(i * ROW_SQUARE_COUNT + j);
}

std::pair<int, int> Board::convert_coordinate_to_indices(std::string coordinate) {
    return std::pair<int, int>(coordinate[0] - 'a', coordinate[1] - '1');
}

std::string Board::convert_indices_to_coordinate(int i, int j) {
    return std::string() + static_cast<char>(i + 'a') + static_cast<char>(j + '1');
}

std::vector<Square *> Board::get_squares_in_fdirection(Square *starting_square, FDirection direction_f, bool one_step) {
    std::vector<Square *> path_squares;
    std::pair<int, int> indices = direction_f(convert_coordinate_to_indices(starting_square->getCoordinate()));

    if (!one_step) {
        while (indices.first >= 0 && indices.second >= 0 && indices.first < ROW_SQUARE_COUNT &&
               indices.second < COLUMN_SQUARE_COUNT) {
            path_squares.push_back(get_square_by_indices_pair(indices));
            indices = direction_f(indices);
        }
    } else {
        path_squares.push_back(get_square_by_indices_pair(indices));
    }

    return path_squares;
}