#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"
#include "Time.h"
#include "Tag.h" 
#include "AP.h" 
//==================================================================
//(Not "\n\n")  - (Yes "\n \n" )
#define SKILL1_SPRITE "assets/img/UI/uiSkillDjinn.png"
#define NAME_SKILL1 "     Ataque pika  "
#define INFO_SKILL1 "5 damage all \n Apply Vulenerable "

#define SKILL2_SPRITE "assets/img/UI/uiSkillNormal.png"
#define NAME_SKILL2 "     Ataque paia  "
#define INFO_SKILL2 "20 damage\n Apply Resilience"

#define SKILL3_SPRITE "assets/img/UI/uiSkillDjinn.png"
#define NAME_SKILL3 "     Defesa top  "
#define INFO_SKILL3 "5 damage \nApply Dodge"

#define SKILL4_SPRITE "assets/img/UI/uiSkillNormal.png"
#define NAME_SKILL4 "     Defesa pika  "
#define INFO_SKILL4 "5 damage"

//-------------------------------------------------------------------
#define SKILL_READER_SPRITE "assets/img/UI/uiSkillReader.png"

#define SKILL_CLICK_COOLDOWN 0.1
//===================================================


class Skill : public Component {
    public:
        enum SkillId {
            SKILL1,
            SKILL2,
            SKILL3,
            SKILL4,
            INVALID_SKILL
        };
        
        enum AttackType {
            ATTACK_INDIVIDUAL,
            ATTACK_ALL,

            DEFENSE_INDIVIDUAL,
            DEFENSE_ALL,

            BUFF_INDIVIDUAL,
            BUFF_ALL,
            
            DEBUFF_INDIVIDUAL,
            DEBUFF_ALL
        };

        enum TargetType {
            MOTHER,
            DAUGHTER,
            IRRELEVANT
        };

        struct SkillInfo {
            int apCost;
            int damage;
            std::vector<Tag::Tags> tags;
            int damageBack;
            std::vector<Tag::Tags> tagsBack;
            std::string name;
            std::string info;
            std::string iconPath;
            AttackType attackType;
            AttackType attackTypeBack;
            TargetType targetTypeBack;
        };



        Skill(GameObject& associated, SkillId id, AP* ap);
        ~Skill();

        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);

        SkillId GetId();

        static Skill* selectedSkill;
        
        static Skill* skillBackToMother;

        void Deselect();
        void DeselectBack(TargetType targetTypeBack);
        void SkillBack(TargetType targetTypeBack);

        static std::map<SkillId, SkillInfo> skillInfoMap; 

        static void InitializeSkillInfoMap();

    private:
        SkillId id;
        Timer skillClickTimer;
        GameObject* readerSkill;
        std::string textSkill = "";
        AP* apInstance;
};
