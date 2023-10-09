#include "Enemies.h"


Enemies::Enemies(GameObject &associated) : Component::Component(associated){
    // Add  sprite
    Sprite *enemies_spr = new Sprite(associated, ENEMEY1_SPRITE);
    enemies_spr->SetScale(0.25, 0.25);
    associated.AddComponent((std::shared_ptr<Sprite>)enemies_spr); 
}

void Enemies::Start() 
{
     
}

Enemies::~Enemies()
{
    
}

void Enemies::Update(float dt)
{   
    
}

void Enemies::Render()
{
}

bool Enemies::Is(std::string type){
    return (type == "Enemies");
}

