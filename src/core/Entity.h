#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <map>
#include <typeinfo>

#include "../../lib/glm/glm.hpp"
#include "./components/Component.h"

class Entity {
  private:
    Entity* parent = NULL;
    std::map<std::string, Entity*> children;
    bool active;
    std::vector<Component*> components;
    std::map<const std::type_info*, Component*> componentTypeMap;

  public:
    std::string name;
    int layer;
    glm::uvec2 parentOffset;
    bool isClickable;
    bool isVisible = true;
    Entity();
    Entity(std::string name, int layer, bool isClickable);
    ~Entity();
    void Update(float deltaTime);
    void Render();
    void Destroy();
    bool IsActive() const;
    std::map<std::string, Entity*> GetChildren() const;
    Entity* GetParent() const;
    void SetParentEntity(Entity* parentEntity);
    void UnsetParentEntity();
    void AddEntityAsChild(Entity* childEntity);
    bool RemoveChild(Entity* childEntity);
    void ListAllComponents() const;
    void SetVisibility(bool visible);

    template <typename T, typename... TArgs>
    T& AddComponent(TArgs&&... args) {
        T* newComponent(new T(std::forward<TArgs>(args)...));
        newComponent->owner = this;
        components.emplace_back(newComponent);
        componentTypeMap[&typeid(*newComponent)] = newComponent;
        newComponent->Initialize();
        return *newComponent;
    }

    template <typename T>
    T* GetComponent() {
        return static_cast<T*>(componentTypeMap[&typeid(T)]);
    }

    template <typename T>
    bool HasComponent() const {
        return componentTypeMap.count(&typeid(T));
    }
};

#endif
