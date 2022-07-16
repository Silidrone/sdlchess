#ifndef CHESS_MTEXTURE_H
#define CHESS_MTEXTURE_H

#include <SDL2/SDL.h>
#include <string>

class Position {
public:
    Position() : m_x(0), m_y(0) {}

    Position(int x, int y) : m_x(x), m_y(y) {}

    void setX(int x) { m_x = x; }

    int getX() const { return m_x; }

    void setY(int y) { m_y = y; }

    int getY() const { return m_y; }

protected:
    int m_x, m_y;
};

class MTexture {
public:
    explicit MTexture(SDL_Renderer *);

    MTexture(const MTexture &);

    ~MTexture();

    bool loadFromFile(std::string);

    void free();

    void setColor(Uint8, Uint8, Uint8);

    void render(SDL_Rect *clip = nullptr);

    void setDestinationRect(SDL_Rect r) { m_destination_rect = r; }

    SDL_Rect getDestinationRect() const { return m_destination_rect; }

private:
    SDL_Texture *m_texture;
    SDL_Renderer *m_renderer;
    SDL_Rect m_destination_rect;
    int *m_instance_count;
};

#endif //CHESS_MTEXTURE_H
