#include "Daughter.h"

#ifdef DEBUG
#include "Camera.h"
#include "Game.h"

#include <SDL2/SDL.h>
#endif //DEBUG

Daughter::Daughter(GameObject &associated) : 
Component::Component(associated),
lifeBarDaughter(nullptr){
    // Add  sprite
    hp = 50;
}

void Daughter::Start() 
{
    Sprite *daughter_spr = new Sprite(associated, DAUGHTER_SPRITE);
    associated.AddComponent((std::shared_ptr<Sprite>)daughter_spr); 
    associated.box.y -= associated.box.h;

    //===================================Hitbox==================================
    daughterHitbox = Rect(associated.box.x, associated.box.y, 130, associated.box.h);

    associated.box.x -= (associated.box.w - daughterHitbox.w )/2;

    //==================================LifeBar====================================
    lifeBarDaughter = new LifeBar(associated, hp, hp, daughterHitbox.w, daughterHitbox.x); //width from hitbox
    associated.AddComponent(std::shared_ptr<LifeBar>(lifeBarDaughter));
}
 
Daughter::~Daughter()
{
    
}

void Daughter::Update(float dt)
{   
    
}

void Daughter::Render()
{
#ifdef DEBUG
    Vec2 center(daughterHitbox.GetCenter());
    SDL_Point points[5];

    Vec2 point = (Vec2(daughterHitbox.x, daughterHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = (Vec2(daughterHitbox.x + daughterHitbox.w, daughterHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[1] = {(int)point.x, (int)point.y};

    point = (Vec2(daughterHitbox.x + daughterHitbox.w, daughterHitbox.y + daughterHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[2] = {(int)point.x, (int)point.y};

    point = (Vec2(daughterHitbox.x, daughterHitbox.y + daughterHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

bool Daughter::Is(std::string type){
    return (type == "Daughter");
}

