#pragma once

#define COMBAT_IMAGE "assets/img/backgrounds/backgroundTemple.png"
#define FONT_COLOR {200, 200, 200, 0} // Red Green Blue Alpha
#define COMBAT_TEXT "PRESS SPACEBAR TO PLAY"

//=========================Positions============================
#define MOTHER_POS Vec2(600.0, (RESOLUTION_HEIGHT) * 2/3 - 375.0)
#define DAUGHTER_POS Vec2(300, RESOLUTION_HEIGHT * 2/3 - 300)
#define ENEMIES_POS1 Vec2(1000, RESOLUTION_HEIGHT * 2/3 - 300)//enemies separeted by x, pos1 only sets the first in the array

#include "State.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Component.h"


class CombatState : public State{
    public:
        CombatState();
        ~CombatState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();
};