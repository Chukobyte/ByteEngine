#include "../Game.h"

SpriteComponent::SpriteComponent(std::string assetTextureId, bool isFixed) {
    this->isAnimated = false;
    this->isFixed = isFixed;
    this->animationFrames = AnimationFrames();
    SetTexture(assetTextureId);
}

SpriteComponent::SpriteComponent(std::string assetTextureId, bool isFixed,  AnimationFrames animationFrames) {
    //this->isAnimated = true;
    this->assetTextureId = assetTextureId;
    this->isFixed = isFixed;
    this->animationFrames = animationFrames;

    SetTexture(this->assetTextureId);

    Play(this->animationFrames.firstAnimationName);
}

SpriteComponent::~SpriteComponent() {
    Game::signalManager->RemoveEntitySignal(signalId);
}

// Only related to animated sprites...
void SpriteComponent::Play(std::string animationName) {
    if(animationName.compare(currentAnimationName) != 0) {
        isAnimated = true;
        Animation animation = animationFrames.GetAnimation(animationName);
        numFrames = animation.numFrames;
        animationIndex = animation.index;
        animationSpeed = animation.animationSpeed;
        currentAnimationName = animation.name;
        SetPlayingAnimationIndex(0);
    }
}

void SpriteComponent::Stop() {
    isAnimated = false;
}

void SpriteComponent::SetTexture(std::string assetTextureId) {
    texture = Game::assetManager->GetTexture(assetTextureId);
}

void SpriteComponent::Initialize() {
    transform = owner->GetComponent<TransformComponent>();
    sourceRectangle.x = 0;
    sourceRectangle.y = 0;
    sourceRectangle.w = transform->width;
    sourceRectangle.h = transform->height;
    signalId = owner->name + "_animation";
    Game::signalManager->CreateSignal(signalId, frameChangedSignalId);
    Game::signalManager->CreateSignal(signalId, animationFinishedSignalId);
}

void SpriteComponent::Update(float deltaTime) {
    if(isAnimated) {
        unsigned int newIndex = static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames);
        if(newIndex != playingAnimationIndex) {
            Game::signalManager->EmitSignal(signalId, frameChangedSignalId);
            if(newIndex + 1 == numFrames) {
                Game::signalManager->EmitSignal(signalId, animationFinishedSignalId);
            }
            sourceRectangle.x = sourceRectangle.w * newIndex;
            playingAnimationIndex = newIndex;
        }
    }
    sourceRectangle.y = animationIndex * transform->height;

    destinationRectangle.x = static_cast<int>(parentTransform == NULL ? transform->position.x : parentTransform->position.x) - (isFixed ? 0 : Game::camera.viewport.x) + owner->parentOffset.x;
    destinationRectangle.y = static_cast<int>(parentTransform == NULL ? transform->position.y : parentTransform->position.y) - (isFixed ? 0 : Game::camera.viewport.y) + owner->parentOffset.y;
    destinationRectangle.w = transform->width * (transform->scale * Game::camera.zoom.x);
    destinationRectangle.h = transform->height * (transform->scale * Game::camera.zoom.y);
}

void SpriteComponent::Render() {
    TextureHandler::Draw(texture, &sourceRectangle, &destinationRectangle);
}

void SpriteComponent::SetParentTransform(TransformComponent* newTransform) {
    parentTransform = newTransform;
}

void SpriteComponent::SetPlayingAnimationIndex(int index) {
    if(index <= numFrames - 1) {
        playingAnimationIndex = index;
        sourceRectangle.x = sourceRectangle.w * playingAnimationIndex;
    }
}
