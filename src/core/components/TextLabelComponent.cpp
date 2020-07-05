#include "TextLabelComponent.h"
#include "../Game.h"
#include "../handlers/FontHandler.h"

TextLabelComponent::TextLabelComponent(std::string text, std::string fontFamily, glm::vec3 color, bool isFixed, Uint32 wrapLength) {
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
    font = Game::assetManager->GetFont(fontFamily);
}

void TextLabelComponent::Update(float deltaTime) {
    if(!isFixed) {
        position.x = static_cast<int>(parentTransform == NULL ? transform->position.x : parentTransform->position.x) - Game::camera.viewport.x + owner->parentOffset.x;
        position.y = static_cast<int>(parentTransform == NULL ? transform->position.y : parentTransform->position.y) - Game::camera.viewport.y + owner->parentOffset.y;
    }
}

void TextLabelComponent::SetLabelText(std::string text) {
    SetLabelText(text, fontFamily);
}

void TextLabelComponent::SetLabelText(std::string text, std::string fontFamily) {
    this->text = text;
}

void TextLabelComponent::Render() {
    //FontHandler::Draw(texture, position);
    FontHandler::Draw(font, text, position.x, position.y, 1.0f, color);
}

void TextLabelComponent::UpdateTransform(TransformComponent* newTransform) {
    parentTransform = newTransform;
}
