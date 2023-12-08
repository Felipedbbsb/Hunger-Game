#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"



#define MESSAGE_NP "assets/img/UI/skills/readers/uiReaderNP.png"

#define UI_NP0 "assets/img/UI/NP00.png" 
#define UI_NP1 "assets/img/UI/NP01.png" 
#define UI_NP2 "assets/img/UI/NP02.png" 

class NP : public Component {
    public:
        NP(GameObject &associated);
        ~NP();
        void Update(float dt);
        void Render();
        void Start();
        void ShowReader();
        void HideReader();
        bool Is(std::string type);
        static void ChangeNPLevel();
        static GameObject* NP_Oj;
    private:
        GameObject* reader;
        
};