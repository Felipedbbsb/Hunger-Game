#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"
#include "Tag.h"

#define TEXT_SKILL_FONT "assets/font/Play-Regular.ttf"
#define TEXT_SKILL_FONT_COLOR {200, 200, 200, 0} // Red Green Blue Alpha



#define TEXT_SKILL_SIZE 15 
#define TagReader_SPRITE "assets/img/UI/uiSkillTagReader.png"

class TagReader : public Component{

    public:
        TagReader(GameObject &associated,
                bool ExposedFeatures,
                std::vector<Tag::Tags> tags,
                Rect reference,
                bool isReversed);

        ~TagReader();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume(); 
        bool Is(std::string type);

    private: 
        bool ExposedFeatures;
        std::vector<Tag::Tags> tags;
        Rect reference; // its the corner of the reader from skill
        bool isReversed;
        std::vector<std::weak_ptr<GameObject>> readerTags;
};