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

#define objectsMoves_VELOCITY 2000

#define OFFSET_SKILL_OPTIONS 250
#define OFFSET_SKILL_OPTIONSY 250

#define SCALE_ARROW 1.1

#define OFFSET_SKILL_SELECTION_ARROW Vec2(50, 50)


#define SKILL_SELECTION_BCKGROUND "assets/img/UI/uiRewardScreen.png"
#define SKILL_SELECTION_MURAL_BCKGROUND "assets/img/UI/uiRewardScreenMural.png"

#define SKILL_SELECTION_ARROW "assets/img/UI/uiRewardScreenArrow.png"
#define SKILL_SELECTION_ARROW_ACTIVATED "assets/img/UI/uiRewardScreenArrowSelected.png"

class SkillSelection : public Component {
    public:
        SkillSelection(GameObject &associated, bool isDjinn);
        ~SkillSelection();
        void Update(float dt);
        void Render();
        void Start(); 
        bool Is(std::string type);
        void CreateSkillOptions();

        void CreateBackground(); 
        void CreatePassButon();

        

        static bool skillSelectionActivated;
        static bool selectionSkillDjinnStyle;
        static bool endSkillSelection;

    private:
        GameObject* background;
        GameObject* passButon;
        GameObject* SkillSelection_obj;
        bool toggleArrow;
        
        float objectsMoves;
        std::vector<std::weak_ptr<GameObject>> rewardArrayObj;

};