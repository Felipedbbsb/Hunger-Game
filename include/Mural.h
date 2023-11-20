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
#define MOTHER_POS Vec2(590.0 + PLAYERSX_STATS_OFFSET, RESOLUTION_HEIGHT * 2/3 + OBJECT_STATS_OFFSET)
#define DAUGHTER_POS Vec2(345 + PLAYERSX_STATS_OFFSET, RESOLUTION_HEIGHT * 2/3 + OBJECT_STATS_OFFSET)
#define ENEMIES_POS1 Vec2(1027, RESOLUTION_HEIGHT * 2/3 + OBJECT_STATS_OFFSET)//enemies separeted by x, pos1 only sets the first in the array

#define OBJECT_STATS_OFFSET -75
 
#define PLAYERSX_STATS_OFFSET -50

#define FOCUS_ENEMY 175

#include <iostream>
#include <memory>
#include <algorithm> 


#include "State.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Component.h"
#include "Enemies.h"

class Mural : public State{
    public:
        Mural(std::string spriteBackground);
        ~Mural();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();
        void CreateBackground(std::string originalPath);


        static bool MuralStateActivateReward;
        static bool MuralState;

    private:
        GameObject* skillSelection; 
        std::string spriteBackground;

        Timer skillSelectionStart;
        Timer skillSelectionEnd;
};