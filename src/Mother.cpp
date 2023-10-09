#include "Mother.h"


// speed já está sendo inicializado pelo construtor de Vec2
Mother::Mother(GameObject &associated) : Component::Component(associated){
    // Add o sprite
    Sprite *mother_spr = new Sprite(associated, MOTHER_SPRITE);
    mother_spr->SetScale(0.125, 0.125);
    associated.AddComponent((std::shared_ptr<Sprite>)mother_spr); 
}

void Mother::Start() 
{
    
}

Mother::~Mother()
{
    
}

void Mother::Update(float dt)
{   
    
}

void Mother::Render()
{
}

bool Mother::Is(std::string type){
    return (type == "Mother");
}

