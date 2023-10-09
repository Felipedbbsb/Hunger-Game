#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"



#define ENEMEY1_SPRITE "assets/img/enemies/enemyChickIdle.png"

class Enemies : public Component{

  public:
    Enemies(GameObject &associated);
    ~Enemies();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();
    bool Is(std::string type);


};