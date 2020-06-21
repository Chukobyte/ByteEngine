#include "ColliderComponent.h"
#include "../Game.h"

ColliderComponent::ColliderComponent(std::string colliderTag, int x, int y, int width, int height, int offsetX, int offsetY) {
    this->colliderTag = colliderTag;
    this->collider = {x, y, width, height};
    this->offset = glm::vec2(offsetX, offsetY);
}

void ColliderComponent::Initialize() {
    if(owner->HasComponent<TransformComponent>()) {
        transform = owner->GetComponent<TransformComponent>();
        sourceRectangle = {0, 0, transform->width, transform->height};
        destinationRectangle = {collider.x, collider.y, collider.w, collider.h};
    }
}

void ColliderComponent::Update(float deltaTime) {
    collider.x = static_cast<int>(transform->position.x + offset.x);
    collider.y = static_cast<int>(transform->position.y + offset.y);
    collider.w = transform->width * transform->scale;
    collider.h = transform->height * transform->scale;
    destinationRectangle.x = collider.x - (transform->scale * Game::camera.zoom.x);
    destinationRectangle.y = collider.y - (transform->scale * Game::camera.zoom.y);
}

void ColliderComponent::Render() {}
