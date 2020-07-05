#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include <SDL2/SDL.h>
#include "../rendering/Texture.h"

class TextureHandler {
  public:
    //static SDL_Texture* LoadTexture(const char* fileName);
    static Texture2D* LoadTexture(const char* fileName);
    static void Draw(Texture2D* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle);
};

#endif
