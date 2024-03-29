#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"
#include "Mother.h"
#include "Daughter.h"
#include "Enemies.h"
#include "Skill.h" 
#include "Vec2.h" 

#define TEXT_SKILL_FONT "assets/font/Play-Regular.ttf"

#define Papiro_EXPOSED_SPRITE "assets/img/UI/uiIndicatorExposed.png"

#define PAPIRO_ENEMY_SPRITE "assets/img/UI/PapiroEnemy.png"
#define PAPIRO_PLAYER_SPRITE "assets/img/UI/PapiroPlayer.png"

#define OFFESET_ENEMIES  Vec2(400, -50)
#define SPACING_ENEMIES  160

#define SPACING_PLAYERS  150

#define PAPIRO_OFFSET 171 // o quanto o papiro sai da tela

#define PAPIRO_SCREEN Vec2(1920 ,638) 
#define BG_SCALE 1 
#define INTERACTION_COOLDOWN 3 

#define OBJECT_VELOCITY (RESOLUTION_WIDTH / 30) * Game::resizer 

class Papiro : public Component {
    public:
        Papiro(GameObject &associated, std::string spriteBackground,  
                std::vector<Enemies::EnemyId> enemiesArrayIS, 
                Skill::AttackType attackType,
                Skill::TargetType whoAttacks, 
                Skill::TargetType whoReceives);
        ~Papiro();
        void Update(float dt);
        void Render();
        void Start();
        bool Is(std::string type);

        void CreateEnemyObject(bool acting);
        void CreatePlayerObject(Skill::TargetType targetType, Skill::TargetType targetReceiver);

    private:
        GameObject* background;
        GameObject* papiro_obj;

        float papiroVelocity;
        float papiroAc;
        std::vector<Enemies::EnemyId> enemiesArrayIS;
        
        Skill::AttackType attackType;
        Skill::TargetType whoAttacks;
        Skill::TargetType whoReceives;
        
        Timer interactionTime;  

        bool movingRight;
        std::string spriteBackground;

        int backgroundOffsetX;

        
        std::vector<std::weak_ptr<GameObject>> PLayerObjects;
        std::vector<std::weak_ptr<GameObject>> interactionObjects;

        bool objectsMovesRight;

        int objectsMoves;

        bool centralized;

};