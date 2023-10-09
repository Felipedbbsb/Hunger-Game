#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"
#include "Time.h"

#define SKILL1_SPRITE "assets/img/UI/uiDjinnSkill.png"
#define SKILL2_SPRITE "assets/img/UI/uiNormalSkill.png"

#define SKILL_READER_SPRITE "assets/img/UI/uiSkillReader.png"

#define SKILL_CLICK_COOLDOWN 0.05

class Skill : public Component{

  public:
    enum SkillId{
        SKILL1, 
        SKILL2
    }; 


    Skill(GameObject &associated, SkillId id);
    ~Skill();
    void Start();                                    
    void Update(float dt);
    void Render();
    bool Is(std::string type);

    SkillId GetId();

  private:
    SkillId id;
    Timer skillClickTimer;
    GameObject* readerSkill;

};