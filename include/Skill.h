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
#define SKILL1_SPRITE "assets/img/UI/uiDjinnSkill.png"
#define TEXT_SKILL1 "     Ataque pika  \n \n Beijo na boca "

#define SKILL2_SPRITE "assets/img/UI/uiNormalSkill.png"
#define TEXT_SKILL2 "     Ataque paia  \n \n Inimigo faz \n 10 polichinelos "

#define SKILL3_SPRITE "assets/img/UI/uiDjinnSkill.png"
#define TEXT_SKILL3 "     Defesa top  \n \n É os guri "

#define SKILL4_SPRITE "assets/img/UI/uiNormalSkill.png"
#define TEXT_SKILL4 "     Defesa pika  \n \n Torcedor do vasco "
//-------------------------------------------------------------------
#define SKILL_READER_SPRITE "assets/img/UI/uiSkillReader.png"

#define SKILL_CLICK_COOLDOWN 0.05
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
    
    struct SkillInfo {
        int damage;
        std::vector<std::string> tags;
        std::string name;
        std::string iconPath;
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
