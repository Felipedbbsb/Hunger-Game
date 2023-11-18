#pragma once

#define SKILL_SELECTION_COOLDOWN_START 2

#define INTENTION_TIME_ANIMATION 1.2f
#define INTENTION_TIME_ANIMATION_V 0.375

#define INDICATOR_TIME_ANIMATION 1.2f
#define INDICATOR_TIME_ANIMATION_V 0.375
#define INDICATOR_ALPHA 125

#define FONT_COLOR {200, 200, 200, 0} // Red Green Blue Alpha
#define COMBAT_TEXT "PRESS SPACEBAR TO PLAY"

//=========================Positions============================
#define MOTHER_POS Vec2(500.0, RESOLUTION_HEIGHT * 2/3 + OBJECT_STATS_OFFSET)
#define DAUGHTER_POS Vec2(200, RESOLUTION_HEIGHT * 2/3 + OBJECT_STATS_OFFSET)
#define ENEMIES_POS1 Vec2(1100, RESOLUTION_HEIGHT * 2/3 + OBJECT_STATS_OFFSET)//enemies separeted by x, pos1 only sets the first in the array

#define OBJECT_STATS_OFFSET -75
 
#define FOCUS_ENEMY 175

#include <iostream>
#include <memory>
#include <algorithm> 


#include "State.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Component.h"
#include "Enemies.h"

class CombatState : public State{
    public:
        CombatState(std::vector<Enemies::EnemyId>, std::string spriteBackground);
        ~CombatState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();
        void CreateBackground(std::string originalPath);
        std::string GeneratePath(std::string originalPath, std::string suffix);
        void CreateEnemies();
        void CreatePlayers();

        static bool InteractionSCreenActivate;
        static std::vector<Enemies::EnemyId> enemiesArrayIS;
        static Skill::AttackType attackType;
        static Skill::TargetType whoAttacks ;
        static Skill::TargetType whoReceives;

        static bool ChangingSides;

    private:
        std::vector<Enemies::EnemyId> enemiesArray;  
        GameObject* papiro;  
        GameObject* skillSelection; 
        std::string spriteBackground;
        bool toggleState;

        Timer skillSelectionStart;
};