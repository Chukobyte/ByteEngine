#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <iostream>
#include <vector>
#include <map>

#include "../../../lib/json/json.hpp"
#include "../../../lib/glm/glm.hpp"

#include "../Config.h"
#include "../helpers/Helper.h"
#include "../AnimationFrames.h"

class ConfigManager {
  private:
    std::string gameTitle;
    std::string initialScenePath;
    std::vector<AssetConfig*> assetConfigs;
    std::map<std::string, AnimationFrames> animationFramesFromConfig;
    std::vector<InputConfig*> inputConfigs;
  public:
    int windowWidth;
    int windowHeight;
    int backgroundRed = 50;
    int backgroundGreen = 50;
    int backgroundBlue = 50;
    glm::uvec2 cameraZoom = glm::uvec2(1.0, 1.0); // Default for now
    std::string pyScriptClassModulePath = "";
    std::string pyScriptClassName = "";

    ConfigManager();
    ConfigManager(nlohmann::json jsonConfig);
    ~ConfigManager();
    std::string GetGameTitle() const;
    std::string GetInitialScenePath() const;
    std::vector<AssetConfig*> GetAssetConfigs() const;
    std::map<std::string, AnimationFrames> GetAnimationFramesFromConfig() const;
    std::vector<InputConfig*> GetInputConfigs() const;
    int GetWindowWidth() const;
    void SetWindowWidth(int windowWidth);
    int GetWindowHeight() const;
    void SetWindowHeight(int windowHeight);
    void ConfigureWindow(nlohmann::json jsonWindow);
    void ConfigureCamera(nlohmann::json jsonCamera);
    std::vector<AssetConfig*> ConfigureAssetConfigs(nlohmann::json jsonAssetArray);
    std::map<std::string, AnimationFrames> ConfigureAnimationFramesConfig(nlohmann::json jsonAnimationFramesArray);
    std::vector<InputConfig*> ConfigureInputConfigs(nlohmann::json jsonInputArray);
    bool ValidateConfig();
};

#endif
