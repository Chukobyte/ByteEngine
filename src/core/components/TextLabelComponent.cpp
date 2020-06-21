#include "TextLabelComponent.h"
#include "../Game.h"
#include "../handlers/FontHandler.h"

TextLabelComponent::TextLabelComponent(std::string text, std::string fontFamily, const SDL_Color& color, bool isFixed, Uint32 wrapLength) {
    this->text = text;
    this->fontFamily = fontFamily;
    this->color = color;
    this->isFixed = isFixed;
    this->wrapLength = wrapLength;
    SetLabelText(text, fontFamily);
}

void TextLabelComponent::Initialize() {
    transform = owner->GetComponent<TransformComponent>();
    position.x = transform->position.x;
    position.y = transform->position.y;
}

void TextLabelComponent::Update(float deltaTime)  {
    if(!isFixed) {
        position.x = static_cast<int>(parentTransform == NULL ? transform->position.x : parentTransform->position.x) - Game::camera.viewport.x + owner->parentOffset.x;
        position.y = static_cast<int>(parentTransform == NULL ? transform->position.y : parentTransform->position.y) - Game::camera.viewport.y + owner->parentOffset.y;
    }
}

void TextLabelComponent::SetLabelText(std::string text) {
    SetLabelText(text, fontFamily);
}

void TextLabelComponent::SetLabelText(std::string text, std::string fontFamily) {
    if(texture != NULL) {
        SDL_DestroyTexture(texture);
    }
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(Game::assetManager->GetFont(fontFamily), text.c_str(), color, wrapLength);
    texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
}

void TextLabelComponent::Render() {
    FontHandler::Draw(texture, position);
}

void TextLabelComponent::UpdateTransform(TransformComponent* newTransform) {
    parentTransform = newTransform;
}
