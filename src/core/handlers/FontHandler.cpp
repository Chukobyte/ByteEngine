#include "FontHandler.h"
#include "../Game.h"

TTF_Font* FontHandler::LoadFont(const char* fileName, int fontSize) {
    TTF_Font* newFont = TTF_OpenFont(fileName, fontSize);
    if(!newFont) {
    	printf("Error loading font at: %s\n", TTF_GetError());
    }
    return newFont;
}

void FontHandler::Draw(SDL_Texture* texture, SDL_Rect position) {
    SDL_RenderCopy(Game::renderer, texture, NULL, &position);
}
