#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "./TransformComponent.h"

class TextLabelComponent : public Component {
  private:
    Uint32 DEFAULT_WRAP_LENGTH = 800;
    SDL_Rect position;
    std::string text;
    std::string fontFamily;
    Uint32 wrapLength;
    SDL_Color color;
    bool isFixed;
    SDL_Texture* texture = NULL;
  public:
    TransformComponent* transform;
    TransformComponent* parentTransform = NULL;

    TextLabelComponent(std::string text, std::string fontFamily, const SDL_Color& color, bool isFixed, Uint32 wrapLength);

    void Initialize() override;

    void Update(float deltaTime) override;

    void SetLabelText(std::string text);

    void SetLabelText(std::string text, std::string fontFamily);

    void Render() override;

    void UpdateTransform(TransformComponent* newTransform);
};

#endif
