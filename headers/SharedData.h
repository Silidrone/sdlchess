#ifndef CHESS_SHAREDDATA_H
#define CHESS_SHAREDDATA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class SharedData {
private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    bool initialized;

    SharedData() : m_window(nullptr), m_renderer(nullptr), initialized(false) {}
public:
    static SharedData &instance() {
        static SharedData INSTANCE;
        return INSTANCE;
    }

    bool init() {
        if (initialized) return false;

        bool success = true;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Set texture filtering to linear
            if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
                printf("Warning: Linear texture filtering not enabled!");
            }

            m_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                        SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            if (m_window == NULL) {
                printf("Window could not be created! %s\n", SDL_GetError());
                success = false;
            } else {
                m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
                if (m_renderer == NULL) {
                    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                    success = false;
                } else {
                    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    int imgFlags = IMG_INIT_PNG;
                    if (!(IMG_Init(imgFlags) & imgFlags)) {
                        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                        success = false;
                    }
                }
            }
        }

        return success;
    }

    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    SDL_Window *getWindow() const { return m_window; }

    SDL_Renderer *getRenderer() const { return m_renderer; }
};

#endif //CHESS_SHAREDDATA_H