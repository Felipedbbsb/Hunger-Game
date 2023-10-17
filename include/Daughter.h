#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"
#include "LifeBar.h"
#include "Skill.h"
#include "Rect.h"


#define DAUGHTER_SPRITE "assets/img/daughter/daughterIdle.png"

class Daughter : public Component{

  public:
    Daughter(GameObject &associated);
    ~Daughter();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();
    bool Is(std::string type);

private:
  LifeBar* lifeBarDaughter;
  Rect daughterHitbox;

  int hp;


};