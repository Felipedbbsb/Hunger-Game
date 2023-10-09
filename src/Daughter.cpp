#include "Daughter.h"


Daughter::Daughter(GameObject &associated) : Component::Component(associated){
    // Add  sprite
    Sprite *daughter_spr = new Sprite(associated, DAUGHTER_SPRITE);
    daughter_spr->SetScale(0.1, 0.1);
    associated.AddComponent((std::shared_ptr<Sprite>)daughter_spr); 
}

void Daughter::Start() 
{
    
}
 
Daughter::~Daughter()
{
    
}

void Daughter::Update(float dt)
{   
    
}

void Daughter::Render()
{
}

bool Daughter::Is(std::string type){
    return (type == "Daughter");
}

