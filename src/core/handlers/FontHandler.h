#ifndef FONTHANDLER_H
#define FONTHANDLER_H

#include "../rendering/Font.h"

class FontHandler {
  public:
    static Font* LoadFont(const char* fileName, int size);

    static void Draw(Font* font, std::string text, float x, float y, float scale, glm::vec3 color);
};

#endif
