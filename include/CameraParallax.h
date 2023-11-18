#pragma once

#include <iostream>

#include "Component.h"
#include "Camera.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Rect.h"

class CameraParallax : public Component{
    public:
        CameraParallax(GameObject& associated, float parallaxFactor);
        void Start(float dt);
        void Update(float dt);
        void Render();
        bool Is(std::string type);

    private:
        int x;
        int y;    
        float parallaxFactor;
};