#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"



#define MOTHER_SPRITE "assets/img/mom/momIdle.png"

class Mother : public Component{

  public:
    Mother(GameObject &associated);
    ~Mother();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();
    bool Is(std::string type);


};