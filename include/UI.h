#pragma once



#include <iostream>
#include <memory>
#include <algorithm> 

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"

#include "Skill.h"

#define UI_SCREEN_SPRITE "assets/img/UI/uiBottomScreen.png"

class UI : public Component{

  public:
    UI(GameObject &associated,
       std::vector<std::shared_ptr<Skill>> SkillNormalArray,
       std::vector<std::shared_ptr<Skill>> SkillDjinArray);

    ~UI();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();
    bool Is(std::string type);

    void AddSkill(std::shared_ptr<Skill> skill);

  private:
    std::vector<std::shared_ptr<Skill>> SkillNormalArray;
    //int nSkillNormal;
    std::vector<std::shared_ptr<Skill>> SkillDjinArray;
    //int nSkillDjinn;

    

};