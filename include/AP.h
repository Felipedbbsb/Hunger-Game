#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"

#define TEXT_SKILL_FONT "assets/font/Play-Regular.ttf"


#define AP_VERTICAL_SPACING 100 

#define AP_QUANTITY 3 

#define AP_FULL_SPRITE "assets/img/UI/uiApFull.png"
#define AP_EMPTY_SPRITE "assets/img/UI/uiApEmpty.png"
#define AP_MIRROR_SPRITE "assets/img/UI/uiApMirror.png"

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
        
        void InitVisualRepresentationForMirror(int mirrorAPCount, GameObject &mirrorObject);

        
};