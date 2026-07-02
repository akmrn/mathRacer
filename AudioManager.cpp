#include "AudioManager.h"

#include <iostream>

AudioManager::AudioManager()
    : device(0),
      mixer(nullptr),
      music(nullptr),
      mixerInitialized(false)
{
    if (!MIX_Init()) {
        std::cout << "MIX init error: " << SDL_GetError() << std::endl;
        return;
    }

    mixerInitialized = true;

    SDL_AudioSpec spec{};
    spec.freq = 44100;
    spec.format = SDL_AUDIO_S16;
    spec.channels = 2;

    device = SDL_OpenAudioDevice(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &spec
    );

    if (!device) {
        std::cout << "Audio device error: " << SDL_GetError() << std::endl;
        return;
    }

    mixer = MIX_CreateMixerDevice(device, &spec);

    if (!mixer) {
        std::cout << "Mixer create error: " << SDL_GetError() << std::endl;
        SDL_CloseAudioDevice(device);
        device = 0;
        return;
    }

    SDL_ResumeAudioDevice(device);
}

AudioManager::~AudioManager()
{
    if (music) {
        MIX_DestroyAudio(music);
        music = nullptr;
    }

    if (mixer) {
        MIX_DestroyMixer(mixer);
        mixer = nullptr;
    }

    if (device) {
        SDL_CloseAudioDevice(device);
        device = 0;
    }

    if (mixerInitialized) {
        MIX_Quit();
        mixerInitialized = false;
    }
}

bool AudioManager::loadMusic(const std::string& path)
{
    if (!mixer) {
        std::cout << "Music load error: mixer is null" << std::endl;
        return false;
    }

    if (music) {
        MIX_DestroyAudio(music);
        music = nullptr;
    }

    music = MIX_LoadAudio(
        mixer,
        path.c_str(),
        true
    );

    if (!music) {
        std::cout << "Music load error for " << path << ": "
                  << SDL_GetError() << std::endl;
        return false;
    }

    std::cout << "Music loaded successfully: " << path << std::endl;
    return true;
}

void AudioManager::playMusic()
{
    if (!mixer) {
        std::cout << "Music play error: mixer is null" << std::endl;
        return;
    }

    if (!music) {
        std::cout << "Music play error: music is null" << std::endl;
        return;
    }

    if (!MIX_PlayAudio(mixer, music)) {
        std::cout << "Music play error: " << SDL_GetError() << std::endl;
        return;
    }

    std::cout << "Music started." << std::endl;
}

void AudioManager::stopMusic()
{
    if (!mixer) {
        return;
    }

    MIX_StopAllTracks(mixer, 0);
}
