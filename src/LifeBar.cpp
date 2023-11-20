#include "LifeBar.h"
#include "Game.h"
#include "Camera.h"
#include "Reader.h"
#include "hpChangeEffect.h"
#include "Text.h"
#include "CameraFollower.h"

#include <cmath>
LifeBar::LifeBar(GameObject& associated, int hpMax, int hpCurrent, int lifeBarWidth, int posx, int hpCorruptedCurrent)
    : Component(associated), 
    hpMax(hpMax),
    hpCurrent(hpCurrent),
    hpCorruptedMax(hpMax),
    hpCorruptedCurrent(hpCorruptedCurrent),
    posx(posx),  
    lifeBarWidth(lifeBarWidth),
    hpReader(nullptr)
    {
    barColor = {150, 15, 15, 255}; // Red color (RGBA)
    corruptedBarColor = {80, 15, 60, 255}; // Cor roxa (RGBA)
    UpdateLifeBarRect(); 
}

LifeBar::~LifeBar() {
    if (hpReader != nullptr){
        hpReader->RequestDelete();
        hpReader = nullptr;
    }  
}
 
void LifeBar::Start() { 
    hpReaderRender();
}

void LifeBar::Update(float dt) { 
    lifeBarRect.x = posx + Camera::pos.x; 
    lifeBarRect.y = static_cast<int>(associated.box.y + associated.box.h + LIFEBAROFFSET + Camera::pos.y);

    corruptedBarRect.x = lifeBarRect.x;
    corruptedBarRect.y = lifeBarRect.y;
}



void LifeBar::Render() {
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    // Calculates the radius of the semicircles based on half the height of the bar
    int semicircleRadius = lifeBarRect.h / 2; 

    
    // Calculate the centers of semicircles 
    int leftSemicircleCenterX = lifeBarRect.x + semicircleRadius;
    int rightSemicircleCenterX = lifeBarRect.x + lifeBarRect.w - semicircleRadius;
    int semicircleCenterY = lifeBarRect.y + lifeBarRect.h / 2; 

    // Sets the color of the semicircles based on the difference between hpMax and hpCurrent
    SDL_Color rightSemicircleColor = (hpMax == hpCurrent) ? barColor : SDL_Color{100, 100, 100, 255};
    if(hpCorruptedCurrent > 0){
        rightSemicircleColor = corruptedBarColor;
    }

    SDL_Color leftSemicircleColor = (hpCurrent > 0) ? barColor : SDL_Color{100, 100, 100, 255};
    if(hpCorruptedCurrent > hpMax){
        leftSemicircleColor = corruptedBarColor;
    }

    // Render the semicircles at the ends of the health bar
    RenderSemicircle(renderer, leftSemicircleCenterX, semicircleCenterY, -semicircleRadius, leftSemicircleColor);
    RenderSemicircle(renderer, rightSemicircleCenterX, semicircleCenterY, semicircleRadius, rightSemicircleColor);

    
    // Render the background of the life bar (empty portion)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Gray color (RGBA)
    SDL_RenderFillRect(renderer, &lifeBarRect); 

    // Render the filled part of the health bar
    SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
    SDL_Rect filledRect = lifeBarRect;
    
    if(hpCurrent > 0){
        filledRect.w = static_cast<int>(static_cast<float>(hpCurrent + 0.1)/ hpMax * lifeBarWidth);
        SDL_RenderFillRect(renderer, &filledRect); 
    }
    
    

    if(hpCorruptedCurrent > 0){
        RenderCorruptedBar(renderer);
    }
    
      
}

void LifeBar::RenderSemicircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color) {
    int numSegments = 180; 

    float angleStep = M_PI / numSegments; // Ângle

    int centerX = x + radius;
    int centerY = y;

    // semicirle
    for (int i = 0; i <= numSegments / 2; i++) {
        float angle = angleStep * i;
        int x1 = static_cast<int>(centerX + radius * std::cos(angle));
        int y1 = static_cast<int>(centerY - radius * std::sin(angle));
        int x2 = static_cast<int>(centerX + radius * std::cos(-angle));
        int y2 = static_cast<int>(centerY - radius * std::sin(-angle));


        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void LifeBar::RenderCorruptedBar(SDL_Renderer* renderer) {
    // Renderiza a parte preenchida da barra de vida corrompida
    SDL_SetRenderDrawColor(renderer, corruptedBarColor.r, corruptedBarColor.g, corruptedBarColor.b, corruptedBarColor.a);
    SDL_Rect corruptedFilledRect = corruptedBarRect;
    int filledWidth = static_cast<int>(static_cast<float>(hpCorruptedCurrent) / hpCorruptedMax * lifeBarWidth);
    corruptedFilledRect.x = corruptedBarRect.x + (lifeBarRect.w - filledWidth);
    corruptedFilledRect.w = filledWidth;
    SDL_RenderFillRect(renderer, &corruptedFilledRect);
}




bool LifeBar::Is(std::string type) {
    return (type == "LifeBar");
}
   
void LifeBar::SetCurrentHP(int hpCurrent) { 

    int hpChange = hpCurrent - this->hpCurrent;
    this->hpCurrent = hpCurrent;
    UpdateLifeBarRect();

    std::string changeText;
    // Create the number change effect GameObject 
    if (hpChange != 0) {

        changeText = (hpChange > 0) ? "+" + std::to_string(hpChange) : std::to_string(hpChange);

    }
    else{ 
        changeText = "DODGE!";
    }
    GameObject *hpChangeEffec_obj = new GameObject(associated.box.x + associated.box.w/2, lifeBarRect.y - associated.box.h); 
    hpChangeEffect* hpReader_behaviour = new hpChangeEffect(*hpChangeEffec_obj, changeText, lifeBarRect.y);
    hpChangeEffec_obj->AddComponent(std::shared_ptr<hpChangeEffect>(hpReader_behaviour));

    hpChangeEffec_obj->box.x -= hpChangeEffec_obj->box.w / 2;
    //hpChangeEffect->box.y += (lifeBarRect.h - hpChangeEffect->box.h) / 2 - 1;

    Game::GetInstance().GetCurrentState().AddObject(hpChangeEffec_obj);

    // Update the HP reader text 
    hpReaderRender();
}

int LifeBar::SetCorruptedHP(int hpCorruptedCurrent) {
    this->hpCorruptedCurrent = hpCorruptedCurrent;
    UpdateLifeBarRect();

    if(hpCorruptedMax - this->hpCorruptedCurrent < hpCurrent){
        SetCurrentHP(hpCorruptedMax - this->hpCorruptedCurrent);
        return hpCorruptedMax - this->hpCorruptedCurrent;
    }
    // Update the HP reader text 
    hpReaderRender();  
    return 0;
    
    
}
 
void LifeBar::hpReaderRender() { 
    if (hpReader != nullptr){ 
        hpReader->RequestDelete();
        hpReader = nullptr;  
    }   
    //Creates Reader for hp
    //position middle of hp bar
    hpReader =  new GameObject(lifeBarRect.x, lifeBarRect.y); //posicao foi no olho...
    std::string textHpReader = std::to_string(hpCurrent) + "/" +std::to_string(hpMax - hpCorruptedCurrent);
    Text* hpReader_behaviour = new Text(*hpReader, TEXT_LIFEBAR_FONT, 
                                                      TEXT_LIFEBAR_SIZE,
                                                      Text::OUTLINE,
                                                      textHpReader, 
                                                      TEXT_LIFEBAR_FONT_COLOR,
                                                      0); 
 
    hpReader->box.x += (lifeBarRect.w - hpReader->box.w)/2 - Camera::pos.x;                                                
    hpReader->box.y += (lifeBarRect.h - hpReader->box.h)/2 - 1 - Camera::pos.y;     

    hpReader->AddComponent(std::shared_ptr<Text>(hpReader_behaviour));
    Game::GetInstance().GetCurrentState().AddObject(hpReader);

}

void LifeBar::UpdateLifeBarRect() {
    // Calculate the position and dimensions of the life bar rectangle
    lifeBarRect.x = static_cast<int>(posx+ Camera::pos.x);
    lifeBarRect.y = static_cast<int>(associated.box.y + associated.box.h + LIFEBAROFFSET+ Camera::pos.y); // Place it above the GameObject
    lifeBarRect.w = lifeBarWidth;
    lifeBarRect.h = lifeBarHeight;  

    corruptedBarRect.x = lifeBarRect.x;
    corruptedBarRect.y = lifeBarRect.y;
    corruptedBarRect.w = lifeBarWidth;
    corruptedBarRect.h = lifeBarHeight;
} 
 