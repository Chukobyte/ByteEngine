#include "AssetManager.h"
#include "../handlers/TextureHandler.h"
#include "../handlers/FontHandler.h"
#include "../handlers/AudioHandler.h"
#include <iostream>

AssetManager::AssetManager() {}

void AssetManager::ClearData() {
    textures.clear();
    fonts.clear();
    music.clear();
    soundEffects.clear();
    shaders.clear();
}

void AssetManager::AddTexture(std::string textureId, const char* filePath) {
    textures.emplace(textureId, TextureHandler::LoadTexture(filePath));
}

void AssetManager::AddFont(std::string fontId, const char* filePath, int fontSize) {
    fonts.emplace(fontId, FontHandler::LoadFont(filePath, fontSize));
}

void AssetManager::AddMusic(std::string musicId, const char* filePath) {
    music.emplace(musicId, AudioHandler::LoadMusic(musicId, filePath));
}

void AssetManager::AddSoundEffect(std::string soundEffectId, const char* filePath) {
    soundEffects.emplace(soundEffectId, AudioHandler::LoadSoundEffect(soundEffectId, filePath));
}

void AssetManager::AddShader(std::string shaderId, const char* vertexPath, const char* fragmentPath) {
    shaders.emplace(shaderId, new Shader(vertexPath, fragmentPath));
}

Texture2D* AssetManager::GetTexture(std::string textureId) {
    return textures[textureId];
}

Font* AssetManager::GetFont(std::string fontId) {
    return fonts[fontId];
}

Mix_Music* AssetManager::GetMusic(std::string musicId) {
    return music[musicId];
}

Mix_Chunk* AssetManager::GetSoundEffect(std::string soundEffectId) {
    return soundEffects[soundEffectId];
}

Shader* AssetManager::GetShader(std::string shaderId) {
    return shaders[shaderId];
}

std::map<std::string, Mix_Chunk*> AssetManager::GetAllSoundEffects() {
    return soundEffects;
}

bool AssetManager::HasMusic(std::string musicId) {
    return music.count(musicId) > 0;
}

bool AssetManager::HasSoundEffect(std::string soundEffectId) {
    return soundEffects.count(soundEffectId) > 0;
}

void AssetManager::ConfigureAssets(std::vector<AssetConfig*> assetConfigs) {
    for(AssetConfig* config : assetConfigs) {
        if(config->type.compare("texture") == 0) {
            AddTexture(config->id, config->filePath.c_str());
        } else if(config->type.compare("music") == 0) {
            AddMusic(config->id, config->filePath.c_str());
        } else if(config->type.compare("sound_effect") == 0) {
            AddSoundEffect(config->id, config->filePath.c_str());
        } else if(config->type.compare("font") == 0) {
            AddFont(config->id, config->filePath.c_str(), config->fontSize);
        }
    }
}
