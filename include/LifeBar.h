#pragma once

#include "Component.h"
#include "GameObject.h"
#include "SDL_include.h"
#include <SDL2/SDL.h>

class LifeBar : public Component {
public:
    LifeBar(GameObject& associated, int hpMax, int hpCurrent);
    ~LifeBar();

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    void SetCurrentHP(int hpCurrent);

private:
    int hpMax;
    int hpCurrent;
    SDL_Color barColor;
    SDL_Rect lifeBarRect;

    const int lifeBarWidth = 100; // Width of the life bar
    const int lifeBarHeight = 10; // Height of the life bar

    void UpdateLifeBarRect();
};
