#include <iostream>
#include "Music.h"
#include "Resources.h"

Music::Music() {
    music = nullptr;
}

Music::Music(std::string file) {
    music = nullptr;
    try {
        Open(file);
    } catch (const std::exception& e) {
        std::cerr << "Failed to create Music: " << e.what() << std::endl;
    }
}

void Music::Play(int times) {
    if (music != nullptr) {
        Mix_PlayMusic(music, times);
    } else {
        std::cerr << "Failed to play music: Music is not loaded!" << std::endl;
    }
}

void Music::Stop(int msToStop) {
    if (music != nullptr) {
        Mix_FadeOutMusic(msToStop);
    } else {
        std::cerr << "Cannot stop music: Music is not loaded!" << std::endl;
    }
}

void Music::Open(std::string file) {
    //music = Mix_LoadMUS(file.c_str()); // const char* file
    music = Resources::GetMusic(file.c_str());
    if (music == nullptr) {
        throw std::runtime_error("Error loading music: " + std::string(Mix_GetError()));
    } else {
        std::cout << "Music uploaded successfully!" << std::endl;
    }
}

bool Music::IsOpen() {
    return (music != nullptr);
}

Music::~Music() {
    if (music != nullptr) {
        //Mix_FreeMusic(music);
        music = nullptr;
    }
}
