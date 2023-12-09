#pragma once



#include <iostream>
#include <memory>
#include <algorithm> 


#include "State.h" 
#include "GameObject.h"
#include "Sprite.h"
#include "Component.h"
#include "Enemies.h"

#define END_BG "assets/img/Backgrounds/menuBg.png"
#define PRESS_SPACE_SPRITE "assets/img/Backgrounds/pressSpacebar.png"

class EndState : public State{
    public:
        EndState();
        ~EndState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();




    private:
        GameObject* pressBar; 

};