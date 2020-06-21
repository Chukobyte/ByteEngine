#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include <string>
#include <typeinfo>

#include "../Entity.h"
#include "../components/Component.h"

class Entity;

class EntityManager {
  private:
    std::vector<Entity*> entities;
    std::map<std::string, Entity*> entitiesToGet;
    std::map<int, std::vector<Entity*>> entitiesToDraw;
    std::vector<int> drawOrderIndex;
    std::vector<Entity*> entitiesToClick;
    std::vector<Entity*> entitiesToDestroy;
    std::map<std::string, Entity*> systemEntities;
    std::vector<Entity*> GetEntitiesByLayer(int layer) const;
    std::vector<Entity*> GetEntitiesWithColliders() const;

  public:
    EntityManager();
    std::vector<Entity*> GetEntities();
    std::vector<std::string> ClearAllEntities();
    void PrepareEntityForDeletion(Entity* entity);
    void Update(float deltaTime);
    void Render();
    bool HasNoEntities();
    void DestroyInactiveEntities();
    Entity& AddEntity(std::string entityName);
    Entity& AddEntityToScene(std::string entityName, int layer, bool isClickable);
    bool AddEntityAsChild(std::string parentEntityId, std::string childEntityId, glm::uvec2 childOffsetPosition);
    Entity& GetEntityByName(std::string entityName);
    Entity* GetEntityPointerByName(std::string entityName);
    std::string GetEntityIdByPosition(int x, int y);
    bool HasEntity(std::string entityId);
    unsigned int GetEntityCount();
    void ListAllEntities() const;
    void ListAllComponents(Entity* entity) const;
    std::vector<Entity*> GetAllSceneEntities() const;
    void ClearEntitiesToDraw();
    void ClearEntitiesToClick();
    //CollisionType CheckCollisions() const;
    //void ToggleEntitiesCollisionDisplay() const;
    void DestroyEntity(Entity* entity);
    void ResetDrawOrderIndex();

    // After refactor
    void AddEntity(Entity* entity);
    void AddSystemEntity(Entity* entity);
};

#endif
