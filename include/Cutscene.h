#pragma once



#include <iostream>
#include <memory>
#include <algorithm> 


#include "State.h" 
#include "GameObject.h"
#include "Sprite.h"
#include "Component.h"
#include "Enemies.h"
#include "Music.h"

#define PATH_CUTSCENES "assets/img/UI/cutscenes"


#define TIME_PER_FRAMECUT 3.75
#define TIME_PER_FADEOUT 1.15

#define CUTS_MAX 13

class Cutscene : public State{
    public:
        Cutscene();
        ~Cutscene();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();

    private:
        void ChangeFrameCut();
        std::string GetFrameSprite();

        GameObject* frameCut;
        int frameCutCount; 
        Timer frameCutTime;
        bool frameTransition;
        
};