#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <string>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool loadMusic(const std::string& path);
    void playMusic();
    void stopMusic();

private:
    SDL_AudioDeviceID device;
    MIX_Mixer* mixer;
    MIX_Audio* music;
    bool mixerInitialized;
};
