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

Piece *HelperFunctions::getChosenPromotedPieceWithModal(ChessColor color, Square *current_square, Board *m_board) {
    auto renderer = SharedData::instance().getRenderer();
    SDL_Event e;
    SDL_Rect square_rect = current_square->getDestination();
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
                    return new Queen(color, current_square, m_board);
                }
                if (SDL_PointInRect(&point, &rook_rect) == SDL_TRUE) {
                    return new Rook(color, current_square, m_board);
                }
                if (SDL_PointInRect(&point, &bishop_rect) == SDL_TRUE) {
                    return new Bishop(color, current_square, m_board);
                }
                if (SDL_PointInRect(&point, &knight_rect) == SDL_TRUE) {
                    return new Knight(color, current_square, m_board);
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