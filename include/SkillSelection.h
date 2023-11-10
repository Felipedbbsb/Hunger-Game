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

#define SKILL_SELECTION_BCKGROUND "assets/img/UI/uiRewardScreen.png"

class SkillSelection : public Component {
    public:
        SkillSelection(GameObject &associated);
        ~SkillSelection();
        void Update(float dt);
        void Render();
        void Start();
        bool Is(std::string type);

        static bool skillSelectionActivated;

    private:
        GameObject* background;
        GameObject* SkillSelection_obj;

        

};