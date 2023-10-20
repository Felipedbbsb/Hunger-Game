#pragma once



#include <iostream>
#include <memory>
#include <algorithm> 

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"
#include "Vec2.h"
#include "Skill.h"

#define AP_POS Vec2( 75, RESOLUTION_HEIGHT * 2/3 + 50) 

#define UI_SCREEN_SPRITE "assets/img/UI/uiBottomScreen.png"

class UI : public Component{

  public:
    UI(GameObject &associated,
       std::vector<Skill::SkillId> SkillNormalArray,
       std::vector<Skill::SkillId> SkillDjinArray);

    ~UI();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();
    bool Is(std::string type);

    void AddSkill(Skill::SkillId skill);

  private:
    std::vector<Skill::SkillId> SkillNormalArray;
    //int nSkillNormal;
    std::vector<Skill::SkillId> SkillDjinArray;
    //int nSkillDjinn;

    

}; 