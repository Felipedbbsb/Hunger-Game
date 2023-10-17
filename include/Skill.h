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

//==================================================================
//(Not "\n\n")  - (Yes "\n \n" )
#define SKILL1_SPRITE "assets/img/UI/uiSkillDjinn.png"
#define NAME_SKILL1 "     Ataque pika  "
#define INFO_SKILL1 "Beijo na boca \n 5 damage all \n Apply Vulenerable and Weak "

#define SKILL2_SPRITE "assets/img/UI/uiSkillNormal.png"
#define NAME_SKILL2 "     Ataque paia  "
#define INFO_SKILL2 "Inimigo faz \n 10 polichinelos \n 20 damage\n Apply Rampage"

#define SKILL3_SPRITE "assets/img/UI/uiSkillDjinn.png"
#define NAME_SKILL3 "     Defesa top  "
#define INFO_SKILL3 "Eh os guri \n 10 damage \nApply Resilience"

#define SKILL4_SPRITE "assets/img/UI/uiSkillNormal.png"
#define NAME_SKILL4 "     Defesa pika  "
#define INFO_SKILL4 "Torcedor do vasco \n 20 damage"

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

        enum SkillsTags {
            RESILIENCE, //Reduz o dano recebido em 50%
            DODGE, //50% de chance de evitar todo o dano do próximo golpe
            PROVOKE,//Força os inimigos a atacarem este alvo
            VULNERABLE,//Aumenta o dano recebido em 50%
            WEAK,//Reduz o seu dano em 50%
            RAMPAGE,//Aumenta seu dano em 50%
            PROTECTED//Não pode ser alvejado
        };

        struct SkillInfo {
            int damage;
            std::vector<SkillsTags> tags;
            std::string name;
            std::string info;
            std::string iconPath;
            AttackType attackType;
        };



        Skill(GameObject& associated, SkillId id);
        ~Skill();

        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);

        SkillId GetId();
        static Skill* selectedSkill;

        void Deselect();

        static std::map<SkillId, SkillInfo> skillInfoMap; 

        static void InitializeSkillInfoMap();

    private:
        SkillId id;
        Timer skillClickTimer;
        GameObject* readerSkill;
        std::string textSkill = "";
};
