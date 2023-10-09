#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"



#define ENEMEY1_SPRITE "assets/img/enemies/enemyChickIdle.png"
#define ENEMEY2_SPRITE "assets/img/enemies/enemyDogIdle.png"
#define ENEMEY3_SPRITE "assets/img/enemies/enemyGhoulIdle.png"
#define ENEMEY4_SPRITE "assets/img/enemies/enemySpiderIdle.png"

class Enemies : public Component{

    public:
        enum EnemyId{
            ENEMY1, 
            ENEMY2,
            ENEMY3, 
            ENEMY4
        }; 

        Enemies(GameObject &associated, EnemyId id);
        ~Enemies();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();
        bool Is(std::string type);

        EnemyId GetId();
    private:
        EnemyId id;

    

};