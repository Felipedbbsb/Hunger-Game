#include "Protected.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Reader.h"
#include "Camera.h"
#include <algorithm> 



bool Protected::isProtected = false; //starts exposed

Protected::Protected(GameObject &associated)
: Component::Component(associated),
isProtectedLocal(Protected::isProtected),
reader(nullptr)
{     
    std::string spritePath = (Protected::isProtected == true) ? PROTECTED_PROTECTED_SPRITE : PROTECTED_EXPOSED_SPRITE;
    Sprite *Protected_spr = new Sprite(associated, spritePath); 
    Protected_spr->SetScale(0.5, 0.5);
    associated.AddComponent((std::shared_ptr<Sprite>)Protected_spr); 
} 
   
void Protected::Start() {     
    
}   
  
Protected::~Protected(){ 
    HideReader();
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
        Protected_spr->SetScale(0.5, 0.5);
        associated.AddComponent((std::shared_ptr<Sprite>)Protected_spr); 
    }
 
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());
 

    if(associated.box.Contains(mousePos.x - Camera::pos.x * Game::resizer, mousePos.y- Camera::pos.y * Game::resizer)){
        ShowReader();
    } else {
        HideReader();
    }
}

void Protected::Render(){
} 


void Protected::ShowReader(){
    if (!reader) {
        reader = new GameObject(associated.box.x, associated.box.y);
        std::string message;
        if(Protected::isProtected){
            message = MESSAGE_PROTECTED;
        }
        else{
            message = MESSAGE_EXPOSED;
        }

        Reader* reader_behaviour = new Reader(*reader, message);
        reader->AddComponent(std::shared_ptr<Reader>(reader_behaviour));
        Game::GetInstance().GetCurrentState().AddObject(reader);
    }
}

void Protected::HideReader() {
    if (reader!= nullptr) {
        reader->RequestDelete();
        reader = nullptr;
    } 
}

bool Protected::Is(std::string type){
    return (type == "Protected");
}

