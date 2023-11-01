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

#define OFFESET_ENEMIES  Vec2(500, -50)
#define SPACING_ENEMIES  100

#define SPACING_PLAYERS  150

#define PAPIRO_SCREEN Vec2(1132, 638) 
#define BG_SCALE 0.5 
#define INTERACTION_COOLDOWN 3

#define OBJECT_VELOCITY PAPIRO_SCREEN.x / 20

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

        void CreateEnemyObject();
        void CreatePlayerObject(Skill::TargetType targetType);

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