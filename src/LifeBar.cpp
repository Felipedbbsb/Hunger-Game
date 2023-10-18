#include "LifeBar.h"
#include "Game.h"
#include "Reader.h"
#include "text.h"

LifeBar::LifeBar(GameObject& associated, int hpMax, int hpCurrent, int lifeBarWidth, int posx )
    : Component(associated), 
    hpMax(hpMax),
    hpCurrent(hpCurrent),
    posx(posx),  
    lifeBarWidth(lifeBarWidth),
    hpReader(nullptr)
    {
    barColor = {255, 0, 0, 255}; // Red color (RGBA)
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
}

void LifeBar::Render() {
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();

    // Render the background of the life bar (empty portion)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Gray color (RGBA)
    SDL_RenderFillRect(renderer, &lifeBarRect);

    // Render the filled portion of the life bar
    SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
    SDL_Rect filledRect = lifeBarRect;
    filledRect.w = static_cast<int>(static_cast<float>(hpCurrent) / hpMax * lifeBarWidth);
    SDL_RenderFillRect(renderer, &filledRect);
    
}

bool LifeBar::Is(std::string type) {
    return (type == "LifeBar");
}
 
void LifeBar::SetCurrentHP(int hpCurrent) {
    this->hpCurrent = hpCurrent;
    UpdateLifeBarRect(); 
    hpReaderRender(); 
}
 
void LifeBar::hpReaderRender() { 
    if (hpReader != nullptr){ 
        hpReader->RequestDelete();
        hpReader = nullptr; 
    }  
    //Creates Reader for hp
    //position middle of hp bar
    hpReader =  new GameObject(lifeBarRect.x, lifeBarRect.y); //posicao foi no olho...
    std::string textHpReader = std::to_string(hpCurrent) + "/" +std::to_string(hpMax);
    Text* hpReader_behaviour = new Text(*hpReader, TEXT_LIFEBAR_FONT, 
                                                      TEXT_LIFEBAR_SIZE,
                                                      Text::OUTLINE,
                                                      textHpReader, 
                                                      TEXT_LIFEBAR_FONT_COLOR,
                                                      0); 
 
    hpReader->box.x += (lifeBarRect.w - hpReader->box.w)/2;                                               
    hpReader->box.y += (lifeBarRect.h - hpReader->box.h)/2 - 1;     

    hpReader->AddComponent(std::shared_ptr<Text>(hpReader_behaviour));
    Game::GetInstance().GetCurrentState().AddObject(hpReader);

}

void LifeBar::UpdateLifeBarRect() {
    // Calculate the position and dimensions of the life bar rectangle
    lifeBarRect.x = static_cast<int>(posx);
    lifeBarRect.y = static_cast<int>(associated.box.y + associated.box.h + LIFEBAROFFSET); // Place it above the GameObject
    lifeBarRect.w = lifeBarWidth;
    lifeBarRect.h = lifeBarHeight;  
} 
 