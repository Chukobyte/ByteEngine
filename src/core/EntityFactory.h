#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <string>

#include "Entity.h"
#include "./managers/EntityManager.h"

class EntityManager;

class EntityFactory {
  private:
    EntityManager* entityManager;
  public:
    EntityFactory() {}

    EntityFactory(EntityManager* entityManager) : entityManager(entityManager) {}

    Entity& Create(std::string entityId, int layer, bool isClickable) {
        Entity* entity = new Entity(entityId, layer, isClickable);
        entityManager->AddEntity(entity);
        return *entity;
    }

    Entity& CreateSystemEntity(std::string entityId) {
        Entity* entity = new Entity(entityId, 0, false);
        entityManager->AddSystemEntity(entity);
        return *entity;
    }
};

#endif
