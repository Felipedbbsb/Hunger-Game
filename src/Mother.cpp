#include "Mother.h"

#ifdef DEBUG
#include "Camera.h"
#include "Game.h"

#include <SDL2/SDL.h>
#endif //DEBUG

// speed já está sendo inicializado pelo construtor de Vec2
Mother::Mother(GameObject &associated) : 
Component::Component(associated),
lifeBarMother(nullptr){

    hp = 50;
}

void Mother::Start() 
{
    Sprite *mother_spr = new Sprite(associated, MOTHER_SPRITE);
    associated.AddComponent((std::shared_ptr<Sprite>)mother_spr); 
    associated.box.y -= associated.box.h;

    //===================================Hitbox==================================
    motherHitbox = Rect(associated.box.x, associated.box.y, 130, associated.box.h);

    associated.box.x -= (associated.box.w - motherHitbox.w )/2;

    //==================================LifeBar====================================
    lifeBarMother = new LifeBar(associated, hp, hp, motherHitbox.w, motherHitbox.x); //width from hitbox
    associated.AddComponent(std::shared_ptr<LifeBar>(lifeBarMother));
}

Mother::~Mother()
{
    
} 

void Mother::Update(float dt)
{   
    
}

void Mother::Render()
{
#ifdef DEBUG
    Vec2 center(motherHitbox.GetCenter());
    SDL_Point points[5];

    Vec2 point = (Vec2(motherHitbox.x, motherHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = (Vec2(motherHitbox.x + motherHitbox.w, motherHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[1] = {(int)point.x, (int)point.y};

    point = (Vec2(motherHitbox.x + motherHitbox.w, motherHitbox.y + motherHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[2] = {(int)point.x, (int)point.y};

    point = (Vec2(motherHitbox.x, motherHitbox.y + motherHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

bool Mother::Is(std::string type){
    return (type == "Mother");
}

