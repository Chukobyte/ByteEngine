#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <string>
#include <SDL2/SDL_mixer.h>

#include "../Game.h"

class AudioHandler {
  public:
    static Mix_Music* LoadMusic(std::string musicId, const char* filePath);
    static Mix_Chunk* LoadSoundEffect(std::string soundEffectId, const char* filePath);
    static void PlayMusic(std::string musicId);
    static void StopMusic();
    static void PauseMusic();
    static void ResumeMusic();
    static void PlaySoundEffect(std::string soundEffectId);
    static void SetVolume(int volume); // Max is 128
};

#endif
