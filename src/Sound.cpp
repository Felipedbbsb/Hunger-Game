#include "Sound.h"
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"
#include "Resources.h"

Sound::Sound(GameObject &associated) : Component::Component(associated) {
    chunk = nullptr;
}

Sound::Sound(GameObject &associated, std::string file) : Sound(associated) {
    chunk = nullptr;
    try {
        Open(file.c_str());
    } catch (const std::exception& e) {
        std::cerr << "Failed to create Sound: " << e.what() << std::endl;
    }
}

void Sound::Play(int times) {
    if (chunk == nullptr) {
        std::cerr << "Failed to play sound: Sound is not loaded!" << std::endl;
        return;
    }

    channel = Mix_PlayChannel(-1, chunk, times - 1); 
    if (channel == -1) {
        std::cerr << "Failed to play sound: " << SDL_GetError() << std::endl;
    }
}

void Sound::Stop() {
    if (chunk != nullptr) {
        Mix_HaltChannel(channel);
    } else {
        std::cerr << "Cannot stop sound: Sound is not loaded!" << std::endl;
    }
}

void Sound::Open(std::string file) {
    //chunk = Mix_LoadWAV(file.c_str());
    chunk = Resources::GetSound(file.c_str());
    if (chunk == nullptr) {
        throw std::runtime_error("Failed to open sound: " + std::string(SDL_GetError()));
    } 
}

Sound::~Sound() {
    if (chunk != nullptr) {
        Mix_HaltChannel(channel);
        chunk = nullptr;
    }
}

bool Sound::Is(std::string type) {
    return (type == "Sound");
}

void Sound::Update(float dt) {
    // If there is any update logic needed for sounds, please add it here.
}

void Sound::Render() {
    // If there are any specific renders for sounds, please add them here.
}
