#include "TextureHandler.h"
#include "../Game.h"

SDL_Texture* TextureHandler::LoadTexture(const char* fileName) {
    SDL_Surface* surface = IMG_Load(fileName);
    if(!surface) {
    	printf("Failed to load image at %s\nSDL_image Error: %s", fileName, IMG_GetError());
    }
    // TODO: setup width and height for textures
    //std::cout << "surface width = " << std::to_string(surface->w) << "surface height = " << std::to_string(surface->h) << std::endl;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void TextureHandler::Draw(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Game::renderer, texture, &sourceRectangle, &destinationRectangle, 0.0, NULL, flip);
}
