#pragma once



#include <iostream>
#include <memory>
#include <algorithm> 


#include "State.h" 
#include "GameObject.h"
#include "Sprite.h"
#include "Component.h"
#include "Enemies.h"
#include "MenuState.h"

#define VICTORY_BG "assets/img/Backgrounds/gameOverBackground.png"
#define VICTORY_HEADER "assets/img/Backgrounds/victoryTitle.png"



class VictoryState : public State{
    public: 
        VictoryState();
        ~VictoryState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();




    private:
        GameObject* pressBar; 
        GameObject* header;

};