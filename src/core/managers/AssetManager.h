#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../Config.h"
#include "../rendering/Texture.h"
#include "../rendering/Font.h"
#include "../rendering/Shader.h"

class AssetManager {
  private:
    std::map<std::string, Texture2D*> textures;
    std::map<std::string, Font*> fonts;
    std::map<std::string, Mix_Music*> music;
    std::map<std::string, Mix_Chunk*> soundEffects;
    std::map<std::string, Shader*> shaders;
  public:
    AssetManager();
    ~AssetManager();
    void ClearData();
    void AddTexture(std::string textureId, const char* filePath);
    void AddFont(std::string fontId, const char* filePath, int fontSize);
    void AddMusic(std::string musicId, const char* filePath);
    void AddSoundEffect(std::string soundEffectId, const char* filePath);
    void AddShader(std::string shaderId, const char* vertexPath, const char* fragmentPath);
    Texture2D* GetTexture(std::string textureId);
    Font* GetFont(std::string fontId);
    Mix_Music* GetMusic(std::string musicId);
    Mix_Chunk* GetSoundEffect(std::string soundEffectId);
    Shader* GetShader(std::string shaderId);
    std::map<std::string, Mix_Chunk*> GetAllSoundEffects();
    bool HasMusic(std::string musicId);
    bool HasSoundEffect(std::string soundEffectId);
    void ConfigureAssets(std::vector<AssetConfig*> assetConfigs);
};

#endif
