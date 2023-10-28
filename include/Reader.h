#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"

#define TEXT_SKILL_FONT "assets/font/Play-Regular.ttf"
#define TEXT_SKILL_FONT_COLOR {200, 200, 200, 0} // Red Green Blue Alpha



#define TEXT_SKILL_SIZE 15 
#define READER_SPRITE "assets/img/UI/uiSkillReader.png"

class Reader : public Component{

    public:
        Reader(GameObject &associated,
                std::string textSkill);

        ~Reader();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();
        bool Is(std::string type);

    private: 
        GameObject* textSkillObj;
        std::string textSkill;
};