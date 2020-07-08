#include "TextureHandler.h"
#include "../Game.h"

Texture2D* TextureHandler::LoadTexture(const char* fileName) {
    Texture2D* texture = new Texture2D(fileName);
    if(!texture->IsValid()) {
        std::cout << "Failed to load texture" << std::endl;
    }
    return texture;
}

void TextureHandler::Draw(Texture2D* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
    Game::spriteRenderer->Draw(texture, sourceRectangle, destinationRectangle);
}
