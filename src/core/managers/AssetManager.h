#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../Config.h"

class AssetManager {
  private:
    std::map<std::string, SDL_Texture*> textures;
    std::map<std::string, TTF_Font*> fonts;
    std::map<std::string, Mix_Music*> music;
    std::map<std::string, Mix_Chunk*> soundEffects;
  public:
    AssetManager();
    ~AssetManager();
    void ClearData();
    void AddTexture(std::string textureId, const char* filePath);
    void AddFont(std::string fontId, const char* filePath, int fontSize);
    void AddMusic(std::string musicId, const char* filePath);
    void AddSoundEffect(std::string soundEffectId, const char* filePath);
    TTF_Font* GetFont(std::string fontId);
    SDL_Texture* GetTexture(std::string textureId);
    Mix_Music* GetMusic(std::string musicId);
    Mix_Chunk* GetSoundEffect(std::string soundEffectId);
    std::map<std::string, Mix_Chunk*> GetAllSoundEffects();
    bool HasMusic(std::string musicId);
    bool HasSoundEffect(std::string soundEffectId);
    void ConfigureAssets(std::vector<AssetConfig*> assetConfigs);
};

#endif
