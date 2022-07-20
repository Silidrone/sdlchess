#include "../headers/Game.h"
#include <SDL2/SDL_image.h>
#include <iostream>

Game::Game() : sharedData(SharedData::instance()), m_board(nullptr) {
    SharedData::instance().init();
}

Game::~Game() {
    //Destroy window
    SDL_DestroyRenderer(sharedData.getRenderer());
    SDL_DestroyWindow(sharedData.getWindow());

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void Game::init() {
    m_board = new Board("../resources/w_square_gray.png", "../resources/b_square_gray.png");
}

void Game::run() {
    SDL_Renderer *renderer = sharedData.getRenderer();
    bool quit = false;

    SDL_Event e;
    bool mouse_clicked = false;
    int mouse_x, mouse_y;
    Piece *selected_piece = nullptr;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mouse_x, &mouse_y);
                selected_piece = m_board->get_square_by_screen_position(mouse_x, mouse_y)->getPiece();
            }
            if (e.type == SDL_MOUSEBUTTONUP) {
                SDL_GetMouseState(&mouse_x, &mouse_y);
                selected_piece->setSquare(m_board->get_square_by_screen_position(mouse_x, mouse_y));
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

        m_board->render();

        //Update screen
        SDL_RenderPresent(renderer);
    }
}
