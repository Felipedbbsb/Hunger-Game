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

#define OFFSET_SKILL_OPTIONS 250
#define OFFSET_SKILL_OPTIONSY 250

#define SCALE_ARROW 1.1

#define OFFSET_SKILL_SELECTION_ARROW Vec2(50, 50)


#define SKILL_SELECTION_BCKGROUND "assets/img/UI/uiRewardScreen.png"

#define SKILL_SELECTION_ARROW "assets/img/UI/uiRewardScreenArrow.png"
#define SKILL_SELECTION_ARROW_ACTIVATED "assets/img/UI/uiRewardScreenArrowSelected.png"

class SkillSelection : public Component {
    public:
        SkillSelection(GameObject &associated, bool isDjinn = false);
        ~SkillSelection();
        void Update(float dt);
        void Render();
        void Start();
        bool Is(std::string type);
        void CreateSkillOptions();

        void CreateBackground();
        void CreatePassButon();

        GameObject* background;
        GameObject* passButon;

        static bool skillSelectionActivated;
        static bool selectionSkillDjinnStyle;

    private:
        
        GameObject* SkillSelection_obj;
        bool toggleArrow;
        

};