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

//NS = Name Skill
//I  = Info
//SPR = Sprite

//MOTHER SKILLS
#define NS_Helmbreaker "Helmbreaker"
#define I_Helmbreaker "Deal 3 damage; \nApply 2 Vulnerable."
#define SPR_Helmbreaker "assets/img/UI/skills/Helmbreaker.png"

#define NS_Rockabye "Rockabye"
#define I_Rockabye "Apply 1 Resilience \n to your daughter."
#define SPR_Rockabye "assets/img/UI/skills/Rockabye.png"

#define NS_Stinger "Stinger"
#define I_Stinger "Deal 5 Damage \n to all enemies; \n Expose your daughter."
#define SPR_Stinger "assets/img/UI/skills/Stinger.png"

//Daughter
#define NS_HnS "Hide and Seek"
#define I_HnS "Apply 1 Dodge \n and 1 Vulnerable \n to Mother; \n Protect Daughter "
#define SPR_HnS "assets/img/UI/skills/HideandSeek.png"

//Djinn
#define NS_InstantRegret "Instant Regret"
#define I_InstantRegret "Deal 15 damage; \n Expose your daughter; \n Apply 1 Vulnerable to your daughter; \n Lose 7HP"
#define SPR_InstantRegret "assets/img/UI/skills/InstantRegret.png"

//Enemies
#define NS_Generic "Attack1"
#define I_Generic ""
#define SPR_Generic ""



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
            
            Helmbreaker,
            Rockabye,
            Stinger,

            HnS,

            InstantRegret,

            E1_Skill1,
            E1_Skill2,
            E1_Skill3,

            E2_Skill1,
            E2_Skill2,
            E2_Skill3,

            E3_Skill1,
            E3_Skill2,

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
            DEBUFF_ALL,

            NONE
        };

        enum TargetType {
            MOTHER,
            DAUGHTER, 
            IRR //irrelevant
        };

        enum StateProtected {
            PROTECTED,
            EXPOSED, 
            NOCHANGES //irrelevant
        };

        struct SkillInfo {
            int apCost;
            StateProtected isProtected;
            int damage;
            std::vector<Tag::Tags> tags;
            int damageBack;
            std::vector<Tag::Tags> tagsBack;
            std::string name;
            std::string info;
            std::string iconPath;
            AttackType attackType;
            TargetType targetTypeAttacker; //who is attacking
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

        static Skill* selectedSkillEnemy;
        
        static Skill* skillBackToMother;

        static Skill* skillBackToDaughter;

        static std::pair<bool, bool> HasTagRampageOrWeak;

        static TargetType playerTargetType;

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
        GameObject* jewelObj;
        bool toggleJewel;
};
