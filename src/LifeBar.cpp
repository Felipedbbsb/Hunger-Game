#include "LifeBar.h"
#include "Game.h"

LifeBar::LifeBar(GameObject& associated, int hpMax, int hpCurrent, int lifeBarWidth, int posx )
    : Component(associated), hpMax(hpMax), hpCurrent(hpCurrent), lifeBarWidth(lifeBarWidth), posx(posx) {
    barColor = {255, 0, 0, 255}; // Red color (RGBA)
    UpdateLifeBarRect();
}

LifeBar::~LifeBar() {}
 
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
}

void LifeBar::UpdateLifeBarRect() {
    // Calculate the position and dimensions of the life bar rectangle
    lifeBarRect.x = static_cast<int>(posx);
    lifeBarRect.y = static_cast<int>(associated.box.y + associated.box.h + LIFEBAROFFSET); // Place it above the GameObject
    lifeBarRect.w = lifeBarWidth;
    lifeBarRect.h = lifeBarHeight;
}
