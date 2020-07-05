#include "SceneManager.h"
#include "../handlers/CollisionHandler.h"
#include "../components/ScriptClassComponent.h"

SceneManager::SceneManager() {}

SceneManager::SceneManager(EntityManager* entityManager) : entityManager(entityManager) {}

SceneManager::~SceneManager() {}

void SceneManager::InitializeScene(Scene newScene) {
    currentScene = newScene;
    // Setup colliders
    for(Entity* entity : entityManager->GetEntities()) {
        AddEntity(entity);
    }
}

void SceneManager::CleanUpScene() {
    entityColliders.clear();
}

void SceneManager::AddEntity(Entity* entity) {
    // Setup colliders
    if(entity->HasComponent<ColliderComponent>()) {
        ColliderComponent* colliderComponent = entity->GetComponent<ColliderComponent>();
        entityColliders.emplace(entity->name, colliderComponent);
    }

    // Execute __create__
    if(entity->HasComponent<ScriptClassComponent>()) {
        ScriptClassComponent* scriptClassComponent = entity->GetComponent<ScriptClassComponent>();
        scriptClassComponent->ExecuteFunction("__create__");
    }
}

void SceneManager::RemoveEntity(std::string entityId) {
    entityColliders.erase(entityId);
}

std::tuple<std::string, std::string> SceneManager::CheckEntityCollisionWithCollider(std::string sourceEntityId, glm::vec2 offset) {
    if(entityColliders.count(sourceEntityId) > 0) {
        for (auto const& colliderRef : entityColliders) {
            std::string targetEntityId  = colliderRef.first;
            if(sourceEntityId.compare(targetEntityId) != 0) {
                ColliderComponent* targetColliderComponent = colliderRef.second;
                if(CollisionHandler::CheckRectangleCollision(entityColliders[sourceEntityId]->collider, targetColliderComponent->collider, offset)) {
                    return std::make_tuple(targetEntityId, targetColliderComponent->colliderTag);
                }
            }
        }
    }
    return std::make_tuple("", "");
}
