#pragma once

#include <iostream>

#include "Component.h"
#include "Camera.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Rect.h"
#include "Timer.h"

#define SCALE_START 1.1

class CameraShake : public Component{
    public:
        CameraShake(GameObject& associated);
        void Start(float dt);
        void Update(float dt);
        void Render();
        bool Is(std::string type);

    private:
        int direction;   
        int x;
        int y;
        Timer shakeTimer;
};