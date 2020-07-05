#include "ConfigManager.h"
#include "../Animation.h"

ConfigManager::ConfigManager() {}

ConfigManager::ConfigManager(nlohmann::json jsonConfig) {
    for(nlohmann::json::iterator it = jsonConfig.begin(); it != jsonConfig.end(); ++it) {
        switch(Helper::hashStringFromConstant(it.key())) {
        case eGameTitle: {
            this->gameTitle = it.value();
            //std::cout << "Game title = " << this->gameTitle << std::endl;
            break;
        }
        case eWindow: {
            nlohmann::json windowJson = it.value();
            ConfigureWindow(windowJson);
            break;
        }
        case eCamera: {
            nlohmann::json cameraJson = it.value();
            ConfigureCamera(cameraJson);
            break;
        }
        case eDefaultScriptClass: {
            nlohmann::json defaultScriptClassJson = it.value();
            this->pyScriptClassModulePath = defaultScriptClassJson["module"].get<std::string>();
            this->pyScriptClassName = defaultScriptClassJson["class"].get<std::string>();;
            break;
        }
        case eInitialScene: {
            this->initialScenePath = it.value();
            //std::cout << "Initial Scene Path = " << this->initialScenePath << std::endl;
            break;
        }
        case eBackgroundColor: {
            nlohmann::json backgroundColorJson = it.value();
            this->backgroundRed = backgroundColorJson["r"].get<int>();
            this->backgroundGreen = backgroundColorJson["g"].get<int>();
            this->backgroundBlue = backgroundColorJson["b"].get<int>();
            break;
        }
        case eAssets: {
            //std::cout << "Assets = " << std::endl;
            nlohmann::json jsonAssets = it.value();
            this->assetConfigs = ConfigureAssetConfigs(jsonAssets);
            break;
        }
        case eAnimationFrames: {
            //std::cout << "Animation Frames = " << std::endl;
            this->animationFramesFromConfig = ConfigureAnimationFramesConfig(it.value());
            break;
        }
        case eInputs: {
            //std::cout << "Inputs = " << std::endl;
            nlohmann::json jsonInputs = it.value();
            this->inputConfigs = ConfigureInputConfigs(jsonInputs);
            break;
        }
        default:
            break;
        }
    }
}

ConfigManager::~ConfigManager() {}

std::string ConfigManager::GetGameTitle() const {
    return gameTitle;
}

int ConfigManager::GetWindowWidth() const {
    return windowWidth;
}

void ConfigManager::SetWindowWidth(int windowWidth) {
    this->windowWidth = windowWidth;
}

int ConfigManager::GetWindowHeight() const {
    return windowHeight;
}

void ConfigManager::SetWindowHeight(int windowHeight) {
    this->windowHeight = windowHeight;
}

std::string ConfigManager::GetInitialScenePath() const {
    return initialScenePath;
}

std::vector<AssetConfig*> ConfigManager::GetAssetConfigs() const {
    return assetConfigs;
}

std::map<std::string, AnimationFrames> ConfigManager::GetAnimationFramesFromConfig() const {
    return animationFramesFromConfig;
}

std::vector<InputConfig*> ConfigManager::GetInputConfigs() const {
    return inputConfigs;
}

void ConfigManager::ConfigureWindow(nlohmann::json jsonWindow) {
    for(nlohmann::json::iterator it = jsonWindow.begin(); it != jsonWindow.end(); ++it) {
        switch(Helper::hashStringFromConstant(it.key())) {
        case eWidth:
            windowWidth = it.value();
            //std::cout << "windowWidth = " << std::to_string(windowWidth) << std::endl;
            break;
        case eHeight:
            windowHeight = it.value();
            //std::cout << "windowHeight = " << std::to_string(windowHeight) << std::endl;
            break;
        default:
            break;
        }
    }
}

void ConfigManager::ConfigureCamera(nlohmann::json jsonCamera) {
    for(nlohmann::json::iterator it = jsonCamera.begin(); it != jsonCamera.end(); ++it) {
        switch(Helper::hashStringFromConstant(it.key())) {
        case eZoom: {
            nlohmann::json zoomJson = it.value();
            float cameraX = zoomJson["x"].get<float>();
            float cameraY = zoomJson["y"].get<float>();
            cameraZoom = glm::uvec2(cameraX, cameraY);
            break;
        }
        default:
            break;
        }
    }
}

std::vector<AssetConfig*> ConfigManager::ConfigureAssetConfigs(nlohmann::json jsonAssetArray) {
    std::vector<AssetConfig*> assets;
    for(nlohmann::json jsonAsset : jsonAssetArray) {
        std::string assetType;
        std::string assetId;
        std::string assetFile;
        int assetFontSize = -1;
        for(nlohmann::json::iterator assetIt = jsonAsset.begin(); assetIt != jsonAsset.end(); ++assetIt) {
            switch(Helper::hashStringFromConstant(assetIt.key())) {
            case eType:
                assetType = assetIt.value();
                break;
            case eId:
                assetId = assetIt.value();
                break;
            case eFile:
                assetFile = assetIt.value();
                break;
            case eFontSize:
                assetFontSize = assetIt.value();
                break;
            default:
                break;
            }
        }
        AssetConfig* assetConfig = new AssetConfig(assetType, assetId, assetFile, assetFontSize);
        //std::cout << "type = " << assetConfig->type << ", id = " << assetConfig->id << ", file = " << assetConfig->filePath << ", font_size = " << std::to_string(assetConfig->fontSize) << std::endl;
        assets.emplace_back(assetConfig);
    }
    return assets;
}

std::map<std::string, AnimationFrames> ConfigManager::ConfigureAnimationFramesConfig(nlohmann::json jsonAnimationFramesArray) {
    std::map<std::string, AnimationFrames> animationFrames;
    for(nlohmann::json jsonAnimationFrame : jsonAnimationFramesArray) {
        std::string animationFrameName;
        std::string animationFrameTextureId;
        nlohmann::json jsonAnimationsArray;
        for(nlohmann::json::iterator animationFrameIt = jsonAnimationFrame.begin(); animationFrameIt != jsonAnimationFrame.end(); ++animationFrameIt) {
            switch(Helper::hashStringFromConstant(animationFrameIt.key())) {
            case eName:
                animationFrameName = animationFrameIt.value();
                break;
            case eTextureId:
                animationFrameTextureId = animationFrameIt.value();
                break;
            case eAnimations:
                jsonAnimationsArray = animationFrameIt.value();
                break;
            default:
                break;
            }
        }

        //std::cout << "name = " << animationFrameName << ", texture_id = " << animationFrameTextureId << std::endl;
        AnimationFrames newAnimationFrames = AnimationFrames(animationFrameName, animationFrameTextureId);
        for(nlohmann::json jsonAnimation : jsonAnimationsArray) {
            std::string animationName;
            unsigned int animationIndex;
            unsigned int animationNumFrames;
            unsigned int animationSpeed;
            for(nlohmann::json::iterator animationIt = jsonAnimation.begin(); animationIt != jsonAnimation.end(); ++animationIt) {
                switch(Helper::hashStringFromConstant(animationIt.key())) {
                case eName:
                    animationName = animationIt.value();
                    break;
                case eIndex:
                    animationIndex = animationIt.value();
                    break;
                case eFrames:
                    animationNumFrames = animationIt.value();
                    break;
                case eSpeed:
                    animationSpeed = animationIt.value();
                    break;
                default:
                    break;
                }
            }
            //std::cout << "animation name = " << animationName << ", animation index = " << std::to_string(animationIndex) << ", animation frames = " << std::to_string(animationNumFrames) << ", animation speed = " << std::to_string(animationSpeed) << std::endl;
            newAnimationFrames.AddAnimation(Animation(animationName, animationIndex, animationNumFrames, animationSpeed));
        }
        animationFrames.emplace(newAnimationFrames.textureId, newAnimationFrames);
    }
    return animationFrames;
}

std::vector<InputConfig*> ConfigManager::ConfigureInputConfigs(nlohmann::json jsonInputArray) {
    std::vector<InputConfig*> inputConfigs;
    for(nlohmann::json jsonInput : jsonInputArray) {
        std::string inputType;
        nlohmann::json jsonActionsArray;
        for(nlohmann::json::iterator inputIt = jsonInput.begin(); inputIt != jsonInput.end(); ++inputIt) {
            switch(Helper::hashStringFromConstant(inputIt.key())) {
            case eType:
                inputType = inputIt.value();
                break;
            case eActions:
                jsonActionsArray = inputIt.value();
                break;
            default:
                break;
            }
        }
        //std::cout << "type = " << inputType << std::endl;
        for(nlohmann::json jsonAction : jsonActionsArray) {
            std::string actionName;
            nlohmann::json jsonInputValuesArray;
            for(nlohmann::json::iterator actionIt = jsonAction.begin(); actionIt != jsonAction.end(); ++actionIt) {
                switch(Helper::hashStringFromConstant(actionIt.key())) {
                case eName:
                    actionName = actionIt.value();
                    break;
                case eInputValues:
                    jsonInputValuesArray = actionIt.value();
                    break;
                default:
                    break;
                }
            }
            //std::cout << "action name = " << actionName << std::endl;
            for(auto& inputValue : jsonInputValuesArray) {
                //std::cout << "input value = " << inputValue << std::endl;
                InputConfig* inputConfig = new InputConfig(inputType, actionName, inputValue);
                inputConfigs.emplace_back(inputConfig);
            }
        }
    }
    return inputConfigs;
}

// TODO: implement actual validation
bool ConfigManager::ValidateConfig() {
    return true;
}
