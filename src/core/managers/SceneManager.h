#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <tuple>

#include "EntityManager.h"
#include "../Scene.h"
#include "../Entity.h"
#include "../components/ColliderComponent.h"

class ColliderComponent;

class SceneManager {
	private:
		EntityManager* entityManager;
		Scene currentScene;
		std::map<std::string, ColliderComponent*> entityColliders;

	public:
		SceneManager();

		SceneManager(EntityManager* entityManager);

		~SceneManager();

		void InitializeScene(Scene newScene);

		void CleanUpScene();

		void AddEntity(Entity* entity);

		void RemoveEntity(std::string entityId);

		// Returns first collision
		std::tuple<std::string, std::string> CheckEntityCollisionWithCollider(std::string sourceEntityId, glm::vec2 offset);
};

#endif
