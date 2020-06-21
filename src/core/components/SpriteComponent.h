#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>

#include "../models/Constants.h"
#include "../handlers/TextureHandler.h"
#include "../managers/AssetManager.h"
#include "../AnimationFrames.h"
#include "../Animation.h"
#include "./TransformComponent.h"

class SpriteComponent: public Component {
  private:
    TransformComponent* transform = NULL;
    TransformComponent* parentTransform = NULL;
    SDL_Texture* texture;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    AnimationFrames animationFrames;
    bool isAnimated;
    int numFrames;
    int animationSpeed;
    bool isFixed;
    std::string currentAnimationName;
    std::string assetTextureId;
    unsigned int animationIndex = 0;
    unsigned int playingAnimationIndex = 0;
    std::string signalId;
    std::string animationFinishedSignalId = "animation_finished";
    std::string frameChangedSignalId = "frame_changed";

  public:
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent(std::string assetTextureId, bool isFixed);
    SpriteComponent(std::string assetTextureId, bool isFixed,  AnimationFrames animationFrames);
    ~SpriteComponent();
    void Play(std::string animationName);
    void Stop();
    void SetTexture(std::string assetTextureId);
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;
    void SetParentTransform(TransformComponent* newTransform);
    void SetPlayingAnimationIndex(int index);
};

#endif
