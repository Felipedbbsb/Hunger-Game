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

#define MENU_BG "assets/img/Backgrounds/menuBg.png"
#define HEADER_MENU_BG "assets/img/Backgrounds/Logo.png"
#define PRESS_SPACE_SPRITE "assets/img/Backgrounds/pressSpacebar.png"
#define APLHA_FADEOUT 1.1

class MenuState : public State{
    public:
        MenuState();
        ~MenuState();
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