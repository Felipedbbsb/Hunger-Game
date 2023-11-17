#pragma once

#include "Component.h"
#include "GameObject.h"
#include "SDL_include.h"
#include "Timer.h"
#include <SDL2/SDL.h>
#include "Reader.h"

#define TEXT_LIFEBAR_FONT "assets/font/Play-Bold.ttf"
#define TEXT_LIFEBAR_FONT_COLOR {255, 255, 255, 0} // Red Green Blue Alpha
#define TEXT_HP_CHANGE_COLOR {255, 255, 255, 0}

#define TEXT_LIFEBAR_SIZE 20
#define TEXT_HP_CHANGE_SIZE 40
#define LIFEBAROFFSET 20 

class LifeBar : public Component {
public:
    LifeBar(GameObject& associated, int hpMax, int hpCurrent, int lifeBarWidth, int posx,
    int hpCorruptedCurrent = 0);
    ~LifeBar();

    void Start();
    void Update(float dt);
    void Render();
    
    bool Is(std::string type); 

    void hpReaderRender();
    void SetCurrentHP(int hpCurrent);
    int SetCorruptedHP(int hpCorruptedCurrent); //return possible damage from Djinn

private:
    int hpMax;
    int hpCurrent;
    SDL_Color barColor;
    SDL_Rect lifeBarRect;

    int hpCorruptedMax;
    int hpCorruptedCurrent;
    SDL_Rect corruptedBarRect;
    SDL_Color corruptedBarColor;

    int posx;
    int lifeBarWidth; // Width of the life bar
    const int lifeBarHeight = 13; // Height of the life bar
    
    void RenderCorruptedBar(SDL_Renderer* renderer);

    void UpdateLifeBarRect();
    void RenderSemicircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color);

    GameObject* hpReader;
    
};
