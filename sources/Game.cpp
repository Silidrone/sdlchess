#include "../headers/Game.h"
#include <SDL2/SDL_image.h>
#include "../headers/SharedData.h"
#include "../headers/Board.h"

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
    m_board = new Board("../resources/wsquare", "../resources/bsquare");
}

void Game::run() {
    SDL_Renderer *renderer = sharedData.getRenderer();
    bool quit = false;

    //Event handler
    SDL_Event e;

    //Modulation components
    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;

    //While application is running
    while (!quit) {
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
                //On keypress change rgb values
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    //Increase red
                    case SDLK_q:
                        r += 32;
                        break;

                        //Increase green
                    case SDLK_w:
                        g += 32;
                        break;

                        //Increase blue
                    case SDLK_e:
                        b += 32;
                        break;

                        //Decrease red
                    case SDLK_a:
                        r -= 32;
                        break;

                        //Decrease green
                    case SDLK_s:
                        g -= 32;
                        break;

                        //Decrease blue
                    case SDLK_d:
                        b -= 32;
                        break;
                }
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
