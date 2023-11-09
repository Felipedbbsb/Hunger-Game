#pragma once

#include "Component.h"
#include "GameObject.h"
#include "SDL_include.h"
#include "Timer.h"
#include <SDL2/SDL.h>
#include "Reader.h"

#define TEXT_hpChangeEffect_FONT "assets/font/Play-Bold.ttf"
#define TEXT_hpChangeEffect_FONT_COLOR {255, 255, 255, 0} // Red Green Blue Alpha
#define TEXT_HP_CHANGE_COLOR {255, 255, 255, 0}

#define TEXT_hpChangeEffect_SIZE 20
#define TEXT_HP_CHANGE_SIZE 40
#define hpChangeEffectOFFSET 20 

#define ANIMATION_DURATION 1.0

class hpChangeEffect : public Component {
public:
    hpChangeEffect(GameObject& associated, std::string number, int lifebarRectY);
    ~hpChangeEffect();

    void Start();
    void Update(float dt);
    void Render();
    
    bool Is(std::string type);

private:
    float animationDuration;
    float maxHeightTime;
    std::string number;
    int lifebarRectY;
    Timer hpChangeEffectTimer;
    int direction; // Random direction (1 for right, -1 for left)
    int concavity; // 1 for upward concavity
    int maxXDisplacement; // Maximum X displacement
    int tMaxHeight;
    float alpha; // Alpha value for transparency

    int initialXPosition;
    int initialYPosition;
};
