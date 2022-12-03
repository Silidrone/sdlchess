#include "../headers/Game.h"
#include <SDL2/SDL_image.h>
#include "../headers/Square.h"
#include "../headers/King.h"
#include "../headers/HelperFunctions.h"

Game::Game() : sharedData(SharedData::instance()), m_moveLogger(), m_board(m_moveLogger),
               m_turn_color(ChessColor::WHITE), m_game_over(false) {}

Game::~Game() {
    //Destroy window
    SDL_DestroyRenderer(sharedData.getRenderer());
    SDL_DestroyWindow(sharedData.getWindow());

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void Game::over() {
    auto renderer = SharedData::instance().getRenderer();
    SDL_SetRenderDrawColor(renderer, 86, 100, 112, 0);
    SDL_RenderClear(renderer);

    MTexture game_over(renderer,
                       std::string() + "Game over " + (m_turn_color == ChessColor::WHITE ? "black" : "white") +
                       " won!", {255, 255, 255});
    auto screen_width = SharedData::instance().SCREEN_WIDTH;
    auto screen_height = SharedData::instance().SCREEN_HEIGHT;
    game_over.render(
            {screen_width * 3 / 12, screen_height / 2 - screen_height / 10, screen_width * 6 / 12, screen_height / 10});
    //Update screen
    SDL_RenderPresent(renderer);
}

void Game::init() {
    SharedData::instance().init();
    m_board.init("../resources/w_square_gray.png", "../resources/b_square_gray.png");
}

void Game::run() {
    SDL_Renderer *renderer = sharedData.getRenderer();
    bool quit = false;

    SDL_Event e;
    int mouse_x, mouse_y;
    Piece *selected_piece = nullptr;
    while (!quit) {
        if (!m_game_over) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_m) {
                    m_moveLogger.toggle();
                }
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    Piece *tmp_piece = m_board.get_square_by_screen_position(mouse_x, mouse_y)->getPiece();
                    if (tmp_piece && m_turn_color == tmp_piece->getColor()) {
                        selected_piece = tmp_piece;
                        selected_piece->setRenderPriority(-1);
                    }
                }
                if (e.type == SDL_MOUSEBUTTONUP && selected_piece) {
                    SDL_GetMouseState(&mouse_x, &mouse_y);

                    if (!selected_piece->move(m_board.get_square_by_screen_position(mouse_x, mouse_y))) {
                        selected_piece->resetPosition();
                    } else {
                        auto previous_turn_color = m_turn_color;
                        m_turn_color = static_cast<ChessColor>(!static_cast<bool>(m_turn_color));
                        m_board.rotate180();

                        auto king = m_board.getKing(m_turn_color);
                        auto king_attacked_squares = king->attacked_squares();
                        if (king->getSquare()->isAttacked(previous_turn_color) &&
                            king->moveable_squares(king_attacked_squares).empty() &&
                            !m_board.legalMoveExists(m_turn_color)) {
                            m_game_over = true;
                        }
                    }
                    selected_piece->setRenderPriority(Piece::DEFAULT_RENDER_PRIORITY);
                    selected_piece = nullptr;
                }
                if (e.type == SDL_MOUSEMOTION && selected_piece) {
                    //Get mouse position
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    selected_piece->setPosition(mouse_x, mouse_y);
                }
            }

            //Clear screen
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer);

            m_board.render();
            m_moveLogger.render();

            //Update screen
            SDL_RenderPresent(renderer);
        } else {
            over();
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
        }
    }
}
