#include "EntityManager.h"
#include <algorithm>
#include <iostream>
//#include "Game.h"
//#include "Collision.h"
//#include "components/ColliderComponent.h"

EntityManager::EntityManager() {}

std::vector<std::string> EntityManager::ClearAllEntities() {
    std::vector<std::string> deletedEntityIds;
    for (auto entityIt = entities.begin(); entityIt != entities.end(); entityIt++) {
        Entity* entity = *entityIt;
        deletedEntityIds.emplace_back(entity->name);
        PrepareEntityForDeletion(entity);
        entityIt--;
    }
    return deletedEntityIds;
}

void EntityManager::DestroyEntity(Entity* entity) {
    PrepareEntityForDeletion(entity);
}

void EntityManager::PrepareEntityForDeletion(Entity* entity) {
    // Unfocus camera if entity with focus is deleted
    //Game::cameraFocus.UnsetObjectTransform(entity->name);

    // Delete child objects first
    /*
    for (auto const& childEntityRef : entity->GetChildren()) {
        Entity* childEntity = childEntityRef.second;
        entity->RemoveChild(childEntity);
        PrepareEntityForDeletion(childEntity);
    }

    // Unset parent
    if(entity->GetParent() != NULL) {
        entity->GetParent()->RemoveChild(entity);
    }
    entity->UnsetParentEntity();
    */

    //Delete self (entity)
    entitiesToGet.erase(entity->name);
    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
    entitiesToClick.erase(std::remove(entitiesToClick.begin(), entitiesToClick.end(), entity), entitiesToClick.end());
    entitiesToDraw[entity->layer].erase(std::remove(entitiesToDraw[entity->layer].begin(), entitiesToDraw[entity->layer].end(), entity), entitiesToDraw[entity->layer].end());
    if(entitiesToDraw.count(entity->layer) <= 0) {
        drawOrderIndex.erase(std::remove(drawOrderIndex.begin(), drawOrderIndex.end(), entity->layer), drawOrderIndex.end());
    }
    entity->Destroy();
    entitiesToDestroy.emplace_back(entity);
}

bool EntityManager::HasNoEntities() {
    return entities.size() == 0;
}

void EntityManager::Update(float deltatTime) {
    for (auto const& entRef : systemEntities) {
        Entity* systemEntity = entRef.second;
        systemEntity->Update(deltatTime);
    }
    for(auto& entity : entities) {
        entity->Update(deltatTime);
    }
    DestroyInactiveEntities();
}

void EntityManager::DestroyInactiveEntities() {
    for(auto& entity : entitiesToDestroy) {
        //std::cout << "EM: deleted entity with id = " << entity->name << std::endl;
        delete entity;
    }
    entitiesToDestroy.clear();
}

void EntityManager::Render() {
    // Not rendering non scene entities for now
    /*
    for (auto const& entRef : systemEntities) {
        Entity* systemEntity = entRef.second;
        systemEntity->Render();
    }
    */

    // Draw Entities
    for(int drawLayer : drawOrderIndex) {
        for(auto& entity : entitiesToDraw[drawLayer]) {
            entity->Render();
        }
    }
}

/*
CollisionType EntityManager::CheckCollisions() const {
    std::vector<Entity*> entitiesWithColliders = GetEntitiesWithColliders();
    for(auto& currentEntity: entitiesWithColliders ) {
        ColliderComponent* currentCollider = currentEntity->GetComponent<ColliderComponent>();
        for(auto& otherEntity : entitiesWithColliders ) {
            if(currentEntity->name.compare(otherEntity->name) != 0) {
                ColliderComponent* otherCollider = otherEntity->GetComponent<ColliderComponent>();
                if(Collision::CheckRectangleCollision(currentCollider->collider, otherCollider->collider)) {
                    if(currentCollider->colliderTag.compare(COLLIDER_TAG_PLAYER) == 0 && otherCollider->colliderTag.compare(COLLIDER_TAG_ENEMY) == 0) {
                        return PLAYER_ENEMY_COLLISION;
                    }
                    if(currentCollider->colliderTag.compare(COLLIDER_TAG_PLAYER) == 0 && otherCollider->colliderTag.compare(COLLIDER_TAG_PROJECTILE) == 0) {
                        return PLAYER_PROJECTILE_COLLISION;
                    }
                    if(currentCollider->colliderTag.compare(COLLIDER_TAG_ENEMY) == 0 && otherCollider->colliderTag.compare(COLLIDER_TAG_FRIENDLY_PROJECTILE) == 0) {
                        return ENEMY_PROJECTILE_COLLISION;
                    }
                    if(currentCollider->colliderTag.compare(COLLIDER_TAG_PLAYER) == 0 && otherCollider->colliderTag.compare(COLLIDER_TAG_LEVEL_COMPLETE) == 0) {
                        return PLAYER_LEVEL_COMPLETE_COLLISION;
                    }
                }
            }
        }
    }
    return NO_COLLISION;
}
*/

void EntityManager::AddEntity(Entity* entity) {
    entities.emplace_back(entity);
    entitiesToGet.emplace(entity->name, entity);
    entitiesToDraw[entity->layer].emplace_back(entity);

    if(entity->isClickable) {
        entitiesToClick.emplace_back(entity);
    }

    // Update and sort draw order
    if(std::find(drawOrderIndex.begin(), drawOrderIndex.end(), entity->layer) == drawOrderIndex.end()) {
        drawOrderIndex.emplace_back(entity->layer);
        std::sort(drawOrderIndex.begin(), drawOrderIndex.end());
    }
    //std::cout << "BE: add entity with id = " << entity->name << std::endl;
}

void EntityManager::AddSystemEntity(Entity* entity) {
    systemEntities.emplace(entity->name, entity);
}

bool EntityManager::AddEntityAsChild(std::string parentEntityId, std::string childEntityId, glm::uvec2 childOffsetPosition) {
    if(HasEntity(parentEntityId) && HasEntity(childEntityId)) {
        entitiesToGet[parentEntityId]->AddEntityAsChild(entitiesToGet[childEntityId]);
        entitiesToGet[childEntityId]->SetParentEntity(entitiesToGet[parentEntityId]);
        entitiesToGet[childEntityId]->parentOffset = childOffsetPosition;
        return true;
    }
    return false;
}

Entity& EntityManager::GetEntityByName(std::string entityName) {
    return *entitiesToGet[entityName];
}

Entity* EntityManager::GetEntityPointerByName(std::string entityName) {
    return entitiesToGet[entityName];
}

std::string EntityManager::GetEntityIdByPosition(int x, int y) {
    /*
    for(auto& entity : entitiesToClick) {
        TransformComponent* entityTransform = entity->GetComponent<TransformComponent>();
        if(x >= entityTransform->position.x
                && x <= entityTransform->position.x +  (entityTransform->width * Game::camera.zoom.x)
                && y >= entityTransform->position.y
                && y <= entityTransform->position.y +  (entityTransform->height * Game::camera.zoom.y)) {
            return entity->name;
        }
    }
    */
    return "";
}

bool EntityManager::HasEntity(std::string entityId) {
    return entitiesToGet.count(entityId) > 0;
}

std::vector<Entity*> EntityManager::GetEntities() {
    return entities;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(int layer) const {
    std::vector<Entity*> selectedEntities;
    for(auto& entity : entities) {
        if(entity->layer == layer) {
            selectedEntities.emplace_back(entity);
        }
    }
    return selectedEntities;
}

std::vector<Entity*> EntityManager::GetEntitiesWithColliders() const {
    std::vector<Entity*> selectedEntities;
    /*
    for(auto& entity : entities) {
        if(entity->HasComponent<ColliderComponent>()) {
            selectedEntities.emplace_back(entity);
        }
    }
    */
    return selectedEntities;
}

unsigned int EntityManager::GetEntityCount() {
    return entities.size();
}

void EntityManager::ClearEntitiesToDraw() {
    entitiesToDraw.clear();
}

void EntityManager::ClearEntitiesToClick() {
    entitiesToClick.clear();
}

void EntityManager::ListAllEntities() const {
    unsigned int i = 0;
    for(auto& entity : entities) {
        entity->ListAllComponents();
        i++;
    }
}

std::vector<Entity*> EntityManager::GetAllSceneEntities() const {
    return entities;
}

/*
void EntityManager::ToggleEntitiesCollisionDisplay() const {
    for(auto& entity : entities) {
        if(entity->HasComponent<ColliderComponent>()) {
            ColliderComponent* colliderComponent = entity->GetComponent<ColliderComponent>();
            colliderComponent->ToggleCollisionBoxRender();
        }
    }
}
*/

void EntityManager::ResetDrawOrderIndex() {
    drawOrderIndex.clear();
}
