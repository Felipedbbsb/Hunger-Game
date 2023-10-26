#include "Protected.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprite.h"
#include <algorithm> 



bool Protected::isProtected = true;

Protected::Protected(GameObject &associated)
: Component::Component(associated),
isProtectedLocal(Protected::isProtected)
{     
} 
   
void Protected::Start() {     
    // Configure the sprites based on the value of isProtected
    std::string spritePath = (Protected::isProtected == true) ? PROTECTED_PROTECTED_SPRITE : PROTECTED_EXPOSED_SPRITE;
    Sprite *Protected_spr = new Sprite(associated, spritePath); 
    Protected_spr->SetScale(0.5, 0.5);
    associated.AddComponent((std::shared_ptr<Sprite>)Protected_spr); 
}   
  
Protected::~Protected(){ 
    
} 

void Protected::Update(float dt){  
    if(isProtectedLocal != Protected::isProtected){
        isProtectedLocal = Protected::isProtected;
        auto spriteComponent = associated.GetComponent("Sprite");
        auto spriteComponentPtr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
        if (spriteComponentPtr) {
            associated.RemoveComponent(spriteComponentPtr);
        }
        std::string spritePath = (Protected::isProtected == true) ? PROTECTED_PROTECTED_SPRITE : PROTECTED_EXPOSED_SPRITE;
        Sprite *Protected_spr = new Sprite(associated, spritePath); // Use shared_ptr
        associated.AddComponent((std::shared_ptr<Sprite>)Protected_spr); 
    }
}

void Protected::Render(){
} 


bool Protected::Is(std::string type){
    return (type == "Protected");
}

