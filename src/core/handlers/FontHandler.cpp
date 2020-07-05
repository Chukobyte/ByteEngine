#include "FontHandler.h"
#include "../Game.h"

/*
TTF_Font* FontHandler::LoadFont(const char* fileName, int fontSize) {
    TTF_Font* newFont = TTF_OpenFont(fileName, fontSize);
    if(!newFont) {
    	printf("Error loading font at: %s\n", TTF_GetError());
    }
    return newFont;
}
*/

Font* FontHandler::LoadFont(const char* fileName, int size) {
    Font* font = new Font(Game::textRenderer->freeTypeLibrary, fileName, size);
    if(!font->IsValid()) {
        std::cout << "Failed to load font" << std::endl;
    }
    return font;
}

/*
void FontHandler::Draw(SDL_Texture* texture, SDL_Rect position) {
    SDL_RenderCopy(Game::renderer, texture, NULL, &position);
}
*/

void FontHandler::Draw(Font* font, std::string text, float x, float y, float scale, glm::vec3 color) {
    Game::textRenderer->Draw(font, text, x, y, scale, color);
}
