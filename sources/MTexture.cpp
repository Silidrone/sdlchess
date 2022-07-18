#include "../headers/MTexture.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <utility>

MTexture::MTexture(SDL_Renderer *renderer) {
    std::cout << "ctor" << std::endl;
    m_texture = nullptr;
    m_renderer = renderer;
    m_instance_count = new int(1);
}

MTexture::MTexture(SDL_Renderer *renderer, std::string path) {
    std::cout << "ctor2" << std::endl;
    m_texture = nullptr;
    m_renderer = renderer;
    m_instance_count = new int(1);
    loadFromFile(std::move(path));
}

MTexture::MTexture(const MTexture &other) {
    std::cout << "copy ctor" << std::endl;
    m_texture = other.m_texture;
    m_renderer = other.m_renderer;
    m_instance_count = other.m_instance_count;
    (*m_instance_count)++;
}

MTexture& MTexture::operator=(const MTexture &other) {
    if(this == &other) return *this;

    std::cout << "assignment operator" << std::endl;
    m_texture = other.m_texture;
    m_renderer = other.m_renderer;
    m_instance_count = other.m_instance_count;
    (*m_instance_count)++;
}

MTexture::~MTexture() {
    std::cout << "dtor" << std::endl;
    free();
    m_renderer = nullptr;
    (*m_instance_count)--;
    if(*m_instance_count == 0) {
        delete m_instance_count;
        m_instance_count = nullptr;
    }
}

bool MTexture::loadFromFile(std::string path) {
    free();

    SDL_Texture *new_texture = nullptr;
    SDL_Surface *loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        new_texture = SDL_CreateTextureFromSurface(m_renderer, loaded_surface);
        if (new_texture == nullptr) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        } else {
            m_width = loaded_surface->w;
            m_height = loaded_surface->h;
        }

        SDL_FreeSurface(loaded_surface);
    }

    m_texture = new_texture;
    return m_texture != nullptr;
}

void MTexture::free() {
    if (m_texture != nullptr && *m_instance_count == 1) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

void MTexture::modulate(Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetTextureColorMod(m_texture, r, g, b);
}

void MTexture::render(int x, int y, int w, int h, SDL_Rect *clip) {
    if(w == -1) w = m_width;
    if(h == -1) h = m_height;

    SDL_Rect render_quad({x, y, w, h});

    if (clip != nullptr) {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }


    SDL_RenderCopy(m_renderer, m_texture, clip, &render_quad);
}

void MTexture::render(SDL_Rect dest, SDL_Rect *clip) {
    render(dest.x, dest.y, dest.w, dest.h, clip);
}