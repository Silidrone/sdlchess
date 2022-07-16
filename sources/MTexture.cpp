#include "../headers/MTexture.h"
#include <SDL2/SDL_image.h>

MTexture::MTexture(SDL_Renderer *renderer) : m_destination_rect({0, 0, 0, 0}) {
    m_texture = nullptr;
    m_renderer = renderer;
    m_instance_count = new int(1);
}

MTexture::MTexture(const MTexture &other) : m_destination_rect(other.m_destination_rect) {
    m_texture = other.m_texture;
    m_renderer = other.m_renderer;
    m_instance_count = other.m_instance_count;
    (*m_instance_count)++;
}

MTexture::~MTexture() {
    free();
    m_renderer = nullptr;
    if (m_instance_count != nullptr) {
        (*m_instance_count)--;
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
            m_destination_rect.w = loaded_surface->w;
            m_destination_rect.h = loaded_surface->h;
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
        m_destination_rect = SDL_Rect();
        delete m_instance_count;
        m_instance_count = nullptr;
    }
}

void MTexture::setColor(Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetTextureColorMod(m_texture, r, g, b);
}

void MTexture::render(SDL_Rect *clip) {
    SDL_Rect render_quad = m_destination_rect;

    if (clip != nullptr) {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    SDL_RenderCopy(m_renderer, m_texture, clip, &render_quad);
}