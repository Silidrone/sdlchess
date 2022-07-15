#include "../headers/MTexture.h"
#include <SDL2/SDL_image.h>

MTexture::MTexture(SDL_Renderer *renderer, Position position) {
	m_texture = NULL;
	m_renderer = renderer;
	m_width = 0;
	m_height = 0;
    setPosition(position);
}

MTexture::~MTexture() {
	free();
	m_renderer = NULL;
}

bool MTexture::loadFromFile(std::string path) {
	free();

	SDL_Texture* new_texture = NULL;
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	if(loaded_surface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	} else {
        new_texture = SDL_CreateTextureFromSurface(m_renderer, loaded_surface);
		if(new_texture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		} else {
			m_width = loaded_surface->w;
			m_height = loaded_surface->h;
		}

		SDL_FreeSurface(loaded_surface);
	}

	m_texture = new_texture;
	return m_texture != NULL;
}

void MTexture::free() {
	if(m_texture != NULL) {
		SDL_DestroyTexture(m_texture);
		m_texture = NULL;
		m_width = 0;
		m_height = 0;
	}
}

void MTexture::setColor(Uint8 r, Uint8 g, Uint8 b) {
	SDL_SetTextureColorMod(m_texture, r, g, b);
}

void MTexture::render(SDL_Rect* clip) {
	SDL_Rect render_quad = {m_position.getX(), m_position.getY(), m_width, m_height};

	if(clip != NULL) {
		render_quad.w = clip->w;
		render_quad.h = clip->h;
	}

	SDL_RenderCopy(m_renderer, m_texture, clip, &render_quad);
}

int MTexture::getWidth() const {
	return m_width;
}

int MTexture::getHeight() const {
	return m_height;
}