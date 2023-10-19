#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"

#define TEXT_SKILL_FONT "assets/font/Play-Regular.ttf"


#define AP_VERTICAL_SPACING 100 


#define AP_FULL_SPRITE "assets/img/UI/uiApFull.png"
#define AP_EMPTY_SPRITE "assets/img/UI/uiApEmpty.png"

class AP : public Component {
    public:
        AP(GameObject &associated);
        ~AP();
        void Update(float dt);
        void Render();
        void Start();
        void SetAPCount(int newAPCount);
        void MirrorAPCount(int mirrorAPCount);

        bool Is(std::string type);

    private:
        void UpdateVisualRepresentation();
        void InitVisualRepresentationForMirror(int mirrorAPCount, GameObject &mirrorObject);

        int apCount;
};