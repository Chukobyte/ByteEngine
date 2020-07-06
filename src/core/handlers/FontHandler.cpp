#include "FontHandler.h"
#include "../Game.h"

Font* FontHandler::LoadFont(const char* fileName, int size) {
    Font* font = new Font(Game::textRenderer->freeTypeLibrary, fileName, size);
    if(!font->IsValid()) {
        std::cout << "Failed to load font" << std::endl;
    }
    return font;
}

void FontHandler::Draw(Font* font, std::string text, float x, float y, float scale, glm::vec3 color) {
    Game::textRenderer->Draw(font, text, x, y, scale, color);
}
