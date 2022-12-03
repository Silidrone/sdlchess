#ifndef CHESS_MTEXTURE_H
#define CHESS_MTEXTURE_H

#include <SDL2/SDL.h>
#include <string>


class MTexture {
public:
    explicit MTexture(SDL_Renderer *);

    explicit MTexture(SDL_Renderer *, std::string);

    explicit MTexture(SDL_Renderer *, std::string, SDL_Color);

    MTexture(const MTexture &);

    MTexture &operator=(const MTexture &);

    ~MTexture();

    bool loadFromText(std::string, SDL_Color);

    bool loadFromFile(std::string);

    void free();

    void modulate(Uint8, Uint8, Uint8);

    void render(SDL_Rect, SDL_Rect * = nullptr);

    void render(int, int, int = -1, int = -1, SDL_Rect * = nullptr);

private:
    SDL_Texture *m_texture;
    SDL_Renderer *m_renderer;
    int *m_instance_count;
    int m_width, m_height;
};

#endif //CHESS_MTEXTURE_H
