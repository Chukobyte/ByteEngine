#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SDL2/SDL.h>
#include <string>

#include "../../../lib/glm/glm.hpp"
#include "../handlers/TextureHandler.h"
#include "TransformComponent.h"

class ColliderComponent : public Component {

  public:
    std::string colliderTag;
    SDL_Rect collider;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    TransformComponent* transform;
    glm::vec2 offset;

    ColliderComponent(std::string colliderTag, int x, int y, int width, int height,  int offsetX, int offsetY);

    void Initialize() override;

    void Update(float deltaTime) override;

    void Render() override;
};

#endif
