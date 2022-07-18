#include "../headers/Game.h"
#include <SDL2/SDL_image.h>

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

    //Event handler
    SDL_Event e;

    //While application is running
    while (!quit) {
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
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
