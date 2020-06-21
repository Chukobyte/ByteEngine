#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include <SDL2/SDL.h>

class TextureHandler {
  public:
    static SDL_Texture* LoadTexture(const char* fileName);
    static void Draw(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip);
};

#endif
