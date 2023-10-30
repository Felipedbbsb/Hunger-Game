#pragma once
#define PAPIRO_VELOCITY 50

#define INTERECTION_COOLDOWN 2.5

#include <iostream>
#include <memory>
#include <algorithm> 

 
#include "State.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Component.h"
#include "Enemies.h"
#include "Timer.h"

class InteractionState : public State{
    public:
        InteractionState(std::vector<Enemies::EnemyId> enemiesArrayIS, 
                        Skill::AttackType attackType, 
                        Skill::TargetType whoAttacks, 
                        Skill::TargetType whoReceives);
        ~InteractionState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();

    private:
        Timer interactionTime;  

        SDL_Texture* screenTexture; // A textura para a tela atual
        SDL_Texture* screenBackup; // A textura para a tela de backup 
        
        void CopyScreenToBackup();

};
 