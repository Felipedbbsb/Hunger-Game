#pragma once

#define INCLUDE_SDL
#define INCLUDE_SDL_MIXER

#include "SDL_include.h"
#include <iostream>

//Class responsible for opening music files and playing them
class Music{
    public:
        Music();
        Music(std::string file);

        void Play(int times = -1);
        void Stop(int msToStop = 1500);
        void Open(std::string file);
        bool IsOpen();

        ~Music();
    private:
        Mix_Music* music;
};