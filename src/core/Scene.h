#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <iostream>

#include "../../lib/json/json.hpp"
#include "./models/Constants.h"
#include "./EntityFactory.h"
#include "./components/TransformComponent.h"
#include "./components/ScriptClassComponent.h"
#include "./components/SpriteComponent.h"
#include "./components/ColliderComponent.h"
#include "./components/TextLabelComponent.h"
#include "./managers/PythonCacheManager.h"
#include "./managers/AnimationFrameManager.h"
#include "./helpers/Helper.h"
#include "Config.h"

class Scene {
  private:
    EntityFactory* entityFactory = NULL;
    PythonCacheManager* pythonCacheManager;
    AnimationFrameManager* animationFrameManager;
    char* defaultModulePath;
    char* defaultClassName;
  public:
    Scene() {}
    Scene(nlohmann::json jsonScene, EntityFactory* entityFactory, PythonCacheManager* pythonCacheManager, AnimationFrameManager* animationFrameManager, char* defaultModulePath, char* defaultClassName) {
        this->entityFactory = entityFactory;
        this->pythonCacheManager = pythonCacheManager;
        this->animationFrameManager = animationFrameManager;
        this->defaultModulePath = defaultModulePath;
        this->defaultClassName = defaultClassName;
        ProcessEntity(jsonScene);
    }

    ProcessEntity(nlohmann::json entityJson) {
        std::string entityId = "";
        int entityLayer = 0;
        nlohmann::json entityComponentsJsonArray = nlohmann::json::array();
        nlohmann::json entityEntitiesJsonArray = nlohmann::json::array();
        std::string externalSceneReference = "";
        bool isClickable = false;
        bool cameraFocuse = false;
        for(nlohmann::json::iterator it = entityJson.begin(); it != entityJson.end(); ++it) {
            switch(Helper::hashStringFromConstant(it.key())) {
            case eId:
                entityId = it.value();
                break;
            case eLayer:
                entityLayer = it.value();
                break;
            case eComponents:
                entityComponentsJsonArray = it.value();
                break;
            case eEntities:
                entityEntitiesJsonArray = it.value();
                break;
            case eExternalSceneReference:
                externalSceneReference = it.value();
                break;
            default:
                break;
            }
        }

        Entity& entity = entityFactory->Create(entityId, entityLayer, isClickable);
        ProcessComponents(entity, entityComponentsJsonArray);
        for(nlohmann::json childEntityJson : entityEntitiesJsonArray) {
            ProcessEntity(childEntityJson);
        }
    }

    // TODO: fix double loop
    ProcessComponents(Entity& entity, nlohmann::json componentsJsonArray) {
        std::map<std::string, nlohmann::json> otherComponentsJsonMap;
        for(nlohmann::json componentJson : componentsJsonArray) {
            std::string componentType = "";
            bool transformCameraFocused = false;
            int transformPositionX = 0;
            int transformPositionY = 0;
            int transformVelocityX = 0;
            int transformVelocityY = 0;
            int transformWidth = 0;
            int transformHeight = 0;
            float transformScale = 1.0;
            int transformRotation = 0;
            for(nlohmann::json::iterator it = componentJson.begin(); it != componentJson.end(); ++it) {
                switch(Helper::hashStringFromConstant(it.key())) {
                case eType:
                    componentType = it.value();
                    break;
                case eCameraFocused:
                    transformCameraFocused = it.value();
                    break;
                case ePosition: {
                    nlohmann::json positionJson = it.value();
                    transformPositionX = positionJson["x"].get<int>();
                    transformPositionY = positionJson["y"].get<int>();
                    break;
                }
                case eVelocity: {
                    nlohmann::json velocityJson = it.value();
                    transformVelocityX = velocityJson["x"].get<int>();
                    transformVelocityY = velocityJson["y"].get<int>();
                    break;
                }
                case eWidth:
                    transformWidth = it.value();
                    break;
                case eHeight:
                    transformHeight = it.value();
                    break;
                case eScale:
                    transformScale = it.value();
                    break;
                case eRotation:
                    transformRotation = it.value();
                    break;
                default:
                    break;
                }
            }

            if(componentType.compare("transform") == 0) {
                entity.AddComponent<TransformComponent>(transformPositionX, transformPositionY, transformVelocityX, transformPositionY, transformWidth, transformHeight, transformScale);
            } else {
                otherComponentsJsonMap.emplace(componentType, componentJson);
            }
        }
        ProcessRemainingComponents(entity, otherComponentsJsonMap);
    }

    ProcessRemainingComponents(Entity& entity, std::map<std::string, nlohmann::json> componentsJsonMap) {
        for (auto const& componentJsonRef : componentsJsonMap) {
            std::string componentType = componentJsonRef.first;
            nlohmann::json componentJson = componentJsonRef.second;
            if(componentType.compare("script_class") == 0) {
                std::string pythonModule;
                std::string pythonClass;
                for(nlohmann::json::iterator it = componentJson.begin(); it != componentJson.end(); ++it) {
                    switch(Helper::hashStringFromConstant(it.key())) {
                    case eModule:
                        pythonModule = it.value();
                        break;
                    case eClass:
                        pythonClass = it.value();
                        break;
                    default:
                        break;
                    }
                }
                entity.AddComponent<ScriptClassComponent>(pythonCacheManager, pythonModule.c_str(), pythonClass.c_str());
            } else if(componentType.compare("sprite") == 0) {
                std::string textureAssetId;
                bool isAnimated = false;
                for(nlohmann::json::iterator it = componentJson.begin(); it != componentJson.end(); ++it) {
                    switch(Helper::hashStringFromConstant(it.key())) {
                    case eTextureId:
                        textureAssetId = it.value();
                        break;
                    case eAnimated:
                        isAnimated = it.value();
                        break;
                    default:
                        break;
                    }
                }
                if(isAnimated && animationFrameManager->HasAnimationFrames(textureAssetId)) {
                    entity.AddComponent<SpriteComponent>(textureAssetId, isAnimated, animationFrameManager->GetAnimationFrames(textureAssetId));
                } else {
                    entity.AddComponent<SpriteComponent>(textureAssetId, isAnimated);
                }
            } else if(componentType.compare("collider") == 0 && entity.HasComponent<TransformComponent>()) {
                TransformComponent* transformComponent = entity.GetComponent<TransformComponent>();
                std::string colliderTag = "";
                int colliderWidth = transformComponent->width;
                int colliderHeight = transformComponent->height;
                int colliderOffsetX = 0;
                int colliderOffsetY = 0;
                for(nlohmann::json::iterator it = componentJson.begin(); it != componentJson.end(); ++it) {
                    switch(Helper::hashStringFromConstant(it.key())) {
                    case eColliderTag: {
                        colliderTag = it.value();
                        break;
                    }
                    case eOffset: {
                        nlohmann::json offsetJson = it.value();
                        colliderOffsetX = offsetJson["x"].get<int>();
                        colliderOffsetY = offsetJson["y"].get<int>();
                        break;
                    }
                    case eSize: {
                        nlohmann::json sizeJson = it.value();
                        colliderWidth = sizeJson["width"].get<int>();
                        colliderHeight = sizeJson["height"].get<int>();
                        break;
                    }
                    default:
                        break;
                    }
                }
                entity.AddComponent<ColliderComponent>(colliderTag, transformComponent->position.x, transformComponent->position.y, colliderWidth, colliderHeight, colliderOffsetX, colliderOffsetY);
            } else if(componentType.compare("text_label") == 0) {
                std::string textLabelText = "";
                std::string textLabelFontId = "";
                int textLabelPositionX = 0;
                int textLabelPositionY = 0;
                glm::vec3 textLabelColor = glm::vec3(1.0f, 1.0f, 1.0f);
                Uint32 textLabelWrapLength = 600;
                bool textLabelIsFixed = false;
                for(nlohmann::json::iterator it = componentJson.begin(); it != componentJson.end(); ++it) {
                    switch(Helper::hashStringFromConstant(it.key())) {
                    case eText: {
                        textLabelText = it.value();
                        break;
                    }
                    case eFontId: {
                        textLabelFontId = it.value();
                        break;
                    }
                    case eWrapLength: {
                        textLabelWrapLength = it.value();
                        break;
                    }
                    case ePosition: {
                        nlohmann::json positionJson = it.value();
                        textLabelPositionX = positionJson["x"].get<int>();
                        textLabelPositionY = positionJson["y"].get<int>();
                        break;
                    }
                    case eColor: {
                        nlohmann::json colorJson = it.value();
                        /*
                        Uint8 red = static_cast<Uint8>(colorJson["r"].get<int>());
                        Uint8 green = static_cast<Uint8>(colorJson["g"].get<int>());
                        Uint8 blue = static_cast<Uint8>(colorJson["b"].get<int>());
                        textLabelColor = {red, green, blue, 0};
                        */
                        float red = static_cast<float>(colorJson["r"].get<int>() * RGB_WHOLE_NUMBER_COLOR_CONVERSION);
                        float green = static_cast<float>(colorJson["g"].get<int>() * RGB_WHOLE_NUMBER_COLOR_CONVERSION);
                        float blue = static_cast<float>(colorJson["b"].get<int>() * RGB_WHOLE_NUMBER_COLOR_CONVERSION);
                        textLabelColor = glm::vec3(red, green, blue);
                        break;
                    }
                    default:
                        break;
                    }
                }
                if(!entity.HasComponent<TransformComponent>()) {
                    entity.AddComponent<TransformComponent>(textLabelPositionX, textLabelPositionY, 0, 0, 0, 0, 1);
                }
                entity.AddComponent<TextLabelComponent>(textLabelText, textLabelFontId, textLabelColor, textLabelIsFixed, textLabelWrapLength);
            }
        }
        // Add Script class at the end
        if(!entity.HasComponent<ScriptClassComponent>()) {
            entity.AddComponent<ScriptClassComponent>(pythonCacheManager, defaultModulePath, defaultClassName);
        }
    }
};

#endif
