#pragma once

#include <iostream>

#include "Component.h"
#include "Camera.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Rect.h"

class CameraFollower : public Component{
    public:
        CameraFollower(GameObject& associated);
        void Start(float dt);
        void Update(float dt);
        void Render();
        bool Is(std::string type);

    private:
        int x;
        int y;    
};