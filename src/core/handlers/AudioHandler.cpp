#include "AudioHandler.h"

Mix_Music* AudioHandler::LoadMusic(std::string musicId, const char* filePath) {
    Mix_Music* newMusic = Mix_LoadMUS(filePath);
    if(!newMusic) {
        printf("Error loading %s music at: %s\n", musicId, filePath);
    }
    return newMusic;
}

Mix_Chunk* AudioHandler::LoadSoundEffect(std::string soundEffectId, const char* filePath) {
    Mix_Chunk* newSound = Mix_LoadWAV(filePath);
    if(!newSound) {
        printf("Error loading %s sound at: %s\n", soundEffectId, filePath);
    }
    return newSound;
}

void AudioHandler::PlayMusic(std::string musicId) {
    if(Game::assetManager->HasMusic(musicId)) {
        Mix_PlayMusic(Game::assetManager->GetMusic(musicId), -1);
    }
}

void AudioHandler::StopMusic() {
    Mix_HaltMusic();
}

void AudioHandler::PauseMusic() {
    Mix_PauseMusic();
}

void AudioHandler::ResumeMusic() {
    Mix_ResumeMusic();
}

void AudioHandler::PlaySoundEffect(std::string soundEffectId) {
    if(Game::assetManager->HasSoundEffect(soundEffectId)) {
        Mix_PlayChannel(-1, Game::assetManager->GetSoundEffect(soundEffectId), 0);
    }
}

// Max is 128
void AudioHandler::SetVolume(int volume) {
    Mix_VolumeMusic(volume);
    for (auto const& mapRef : Game::assetManager->GetAllSoundEffects()) {
        Mix_Chunk* soundEffectChunk = mapRef.second;
        Mix_VolumeChunk(soundEffectChunk, volume);
    }
}
