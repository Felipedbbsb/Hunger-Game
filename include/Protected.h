#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"

#define TEXT_SKILL_FONT "assets/font/Play-Regular.ttf"

#define PROTECTED_EXPOSED_SPRITE "assets/img/UI/uiIndicatorExposed.png"

#define PROTECTED_PROTECTED_SPRITE "assets/img/UI/uiIndicatorProtected.png"

#define MESSAGE_PROTECTED "assets/img/UI/tagReaders/tagReaderProtected.png"

#define MESSAGE_EXPOSED "assets/img/UI/tagReaders/tagReaderExposed.png"

class Protected : public Component {
    public:
        Protected(GameObject &associated);
        ~Protected();
        void Update(float dt);
        void Render();
        void Start();
        void ShowReader();
        void HideReader();
        bool Is(std::string type);

        static bool isProtected;

    private:
        bool isProtectedLocal;
        GameObject* reader;
};