#ifndef FONTHANDLER_H
#define FONTHANDLER_H

#include <SDL2/SDL_ttf.h>

class FontHandler {
  public:
    static TTF_Font* LoadFont(const char* fileName, int fontSize);

    static void Draw(SDL_Texture* texture, SDL_Rect position);
};

#endif
