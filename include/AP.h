#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"
#include "Vec2.h"
#include "Timer.h"

#define TEXT_SKILL_FONT "assets/font/Play-Regular.ttf"


#define AP_VERTICAL_SPACING 100 

#define AP_QUANTITY 3 

#define AP_FULL_SPRITE "assets/img/UI/uiApFull.png"
#define AP_EMPTY_SPRITE "assets/img/UI/uiApEmpty.png"
#define AP_MIRROR_SPRITE "assets/img/UI/uiApMirror.png"

#define AP_POS1 Vec2(222, 210)
#define AP_POS2 Vec2(279, 180)
#define AP_POS3 Vec2(222, 146)

#define DELAY_CHANGE_SIDES 1

class AP : public Component {
    public:
        AP(GameObject &associated);
        ~AP();
        void Update(float dt);
        void Render();
        void Start();
        void SetAPCount(int newAPCount);
        void MirrorAPCount(int mirrorAPCount);
        void UpdateVisualRepresentation();
        bool Is(std::string type);

        static std::vector<std::shared_ptr<GameObject>> apArray;
        static int apCount;
    private:
        Timer delayChangeSides;    

};