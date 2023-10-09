#pragma once

#define TITLE_IMAGE "assets/img/Backgrounds/backgroundTemple.png"
#define TITLE_FONT "assets/font/Call me maybe.ttf"
#define FONT_COLOR {200, 200, 200, 0} // Red Green Blue Alpha
#define TITLE_TEXT "PRESS SPACEBAR TO PLAY"



#include "State.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Component.h"


class TitleState : public State{
    public:
        TitleState();
        ~TitleState();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();
};