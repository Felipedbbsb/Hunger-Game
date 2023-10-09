#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"



#define DAUGHTER_SPRITE "assets/img/Daughter/daughterIdle.png"

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


};