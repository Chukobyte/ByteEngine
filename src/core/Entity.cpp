#include "Entity.h"

Entity::Entity() {
    this->active = true;
    this->isClickable = false;
    this->parentOffset = glm::uvec2(0, 0);
}

Entity::Entity(std::string name, int layer, bool isClickable): name(name), layer(layer), isClickable(isClickable) {
    this->active = true;
    this->parentOffset = glm::uvec2(0, 0);
}

Entity::~Entity() {}

void Entity::Update(float deltaTime) {
    for(auto& component: components) {
        component->Update(deltaTime);
    }
}

void Entity::Render() {
    if(isVisible) {
        for(auto& component: components) {
            component->Render();
        }
    }
}

void Entity::Destroy() {
    this->active = false;
    componentTypeMap.clear();
    for(auto& component: components) {
        delete component;
    }
    components.clear();
    children.clear();
}

bool Entity::IsActive() const {
    return this->active;
}

std::map<std::string, Entity*> Entity::GetChildren() const {
    return children;
}
Entity* Entity::GetParent() const {
    return parent;
}

void Entity::SetParentEntity(Entity* parentEntity) {
    parent = parentEntity;
    /*
    if(parent->HasComponent<TransformComponent>() && HasComponent<TransformComponent>()) {
        TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
        for(auto* component : components) {
            component->UpdateTransform(parentTransform);
        }
    }
    */
}

void Entity::UnsetParentEntity() {
    parent = NULL;
}

void Entity::AddEntityAsChild(Entity* childEntity) {
    children.emplace(childEntity->name, childEntity);
}

bool Entity::RemoveChild(Entity* childEntity) {
    if(children.count(childEntity->name) > 0) {
        children.erase(childEntity->name);
        return true;
    }
    return false;
}

void Entity::ListAllComponents() const {

}

void Entity::SetVisibility(bool visible) {
    this->isVisible = visible;
}
