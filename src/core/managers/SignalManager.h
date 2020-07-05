#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H

#include "EntityManager.h"
#include "../components/ScriptClassComponent.h"

class Subscriber {
  public:
    std::string entityId;
    std::string functionName;
    Subscriber(std::string entityId, std::string functionName) : entityId(entityId), functionName(functionName) {}
};

class Signal {
  public:
    std::string id;
    std::string ownerId;
    std::map<std::string, Subscriber> subscribers;
    EntityManager* entityManager;

    Signal() {}
    Signal(std::string id, std::string ownerId, EntityManager* entityManager) : id(id), ownerId(ownerId), entityManager(entityManager) {}
    ~Signal() {}

    void AddSubscriber(std::string subscriberEntityId, std::string subscriberFunctionName) {
        subscribers.emplace(subscriberEntityId, Subscriber(subscriberEntityId, subscriberFunctionName));
    }

    void Emit() {
        for(auto const& subscriberRef : subscribers) {
            Subscriber subscriber = subscriberRef.second;
            if(entityManager->HasEntity(subscriber.entityId)) {
                Entity* entity = entityManager->GetEntityPointerByName(subscriber.entityId);
                if(entity->HasComponent<ScriptClassComponent>()) {
                    ScriptClassComponent* scriptClassComponent = entity->GetComponent<ScriptClassComponent>();
                    scriptClassComponent->ExecuteFunction(subscriber.functionName);
                }
            } else {
                // Clean up if subscriber doesn't exist
                subscribers.erase(subscriberRef.first);
            }
        }
    }
};

class EntitySignals {
  public:
    std::string entityId;
    std::map<std::string, Signal> signals;
    EntityManager* entityManager;

    EntitySignals() {}
    EntitySignals(std::string entityId, EntityManager* entityManager) : entityId(entityId), entityManager(entityManager) {}
    ~EntitySignals() {}

    void CreateSignal(std::string signalId) {
        if(signals.count(signalId) <= 0) {
            signals.emplace(signalId, Signal(signalId, entityId, entityManager));
        }
    }

    void Subscribe(std::string signalId, std::string subscriberEntityId, std::string subscriberFunctionName) {
        signals[signalId].AddSubscriber(subscriberEntityId, subscriberFunctionName);
    }

    void EmitSignal(std::string signalId) {
        signals[signalId].Emit();
    }
};

class SignalManager {
  public:
    std::map<std::string, EntitySignals> entitySignals;
    EntityManager* entityManager;

    SignalManager(EntityManager* entityManager) : entityManager(entityManager) {}

    SignalManager() {}

    ~SignalManager() {}

    void CreateSignal(std::string entityId, std::string signalId) {
        if(entitySignals.count(entityId) <= 0) {
            entitySignals.emplace(entityId, EntitySignals(entityId, entityManager));
        }
        entitySignals[entityId].CreateSignal(signalId);
    }

    void SubscribeToSignal(std::string sourceId, std::string signalId, std::string subscriberEntityId, std::string subscriberFunctionName) {
        CreateSignal(sourceId, signalId); // Creates if doesn't exist
        entitySignals[sourceId].Subscribe(signalId, subscriberEntityId, subscriberFunctionName);
    }

    void EmitSignal(std::string entityId, std::string signalId) {
        if(entitySignals.count(entityId) > 0) {
            entitySignals[entityId].EmitSignal(signalId);
        }
    }

    void RemoveEntitySignal(std::string entityId) {
        entitySignals.erase(entityId);
    }
};

#endif
