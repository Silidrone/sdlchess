#ifndef CHESS_MTEXTURE_H
#define CHESS_MTEXTURE_H

#include <SDL2/SDL.h>
#include <string>

class Position {
public:
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
    explicit MTexture(SDL_Renderer *, Position = Position(0, 0));

    ~MTexture();

    bool loadFromFile(std::string);

    void free();

    void setColor(Uint8, Uint8, Uint8);

    void render(SDL_Rect *clip = NULL);

    int getWidth() const;

    int getHeight() const;

    void setPosition(Position position) { m_position = position; }
    Position getPosition() const { return m_position; }

private:
    SDL_Texture *m_texture;
    SDL_Renderer *m_renderer;
    int m_width, m_height;
    Position m_position;
};

#endif //CHESS_MTEXTURE_H
