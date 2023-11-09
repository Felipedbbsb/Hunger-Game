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

#define AP_POS Vec2( 0 , RESOLUTION_HEIGHT * 2/3) 

#define PROTECTED_POS Vec2( 250, RESOLUTION_HEIGHT/10 + 75) 

#define SKILL_N_OFFSET Vec2( 367, RESOLUTION_HEIGHT * 2/3 + 123) 

#define SKILL_D_OFFSET Vec2( 200, RESOLUTION_HEIGHT * 2/3 + 123) //x = distance between normal and djinn skills

#define SKILL_SPACE 174

#define UI_SCREEN_SPRITE "assets/img/UI/uiBottomScreen.png" 

#define UI_SCREEN_BG_SPRITE "assets/img/UI/uiBottomScreenBG.png"

class UI : public Component{

  public:
    UI(GameObject &associated);

    ~UI();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();
    bool Is(std::string type);


  private:
  

    

}; 