#ifndef CHESS_HELPERFUNCTIONS_H
#define CHESS_HELPERFUNCTIONS_H

#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_ttf.h>

void getTextureFromText(std::string text, SDL_Color color = {255, 255, 255},) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(Sans, text.c_str(), color);

}

#endif //CHESS_HELPERFUNCTIONS_H
