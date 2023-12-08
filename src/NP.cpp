#include "NP.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "GameData.h"
#include "Sprite.h"
#include "Reader.h"
#include "Camera.h"
#include "CameraFollower.h"
#include <algorithm> 

GameObject* NP::NP_Oj = nullptr;

NP::NP(GameObject &associated)
: Component::Component(associated),
reader(nullptr)
{     

    NP_Oj = new GameObject(0, 0);

    std::string stringPath;
    if(GameData::npLevel == 0){
        stringPath = UI_NP0;
    }
    else if(GameData::npLevel == 1){
        stringPath = UI_NP1;
    }
    else if(GameData::npLevel == 2){
        stringPath = UI_NP2;
    }
    else{
        stringPath = UI_NP2;
    }

    Sprite* NP_spr = new Sprite(*NP_Oj, stringPath); 
    NP_spr->SetAlpha(225);
    NP_Oj->AddComponent((std::shared_ptr<Sprite>)NP_spr); 

    NP_spr->SetScale(0.75, 0.75);
    NP_Oj->box.x = RESOLUTION_WIDTH/2 - NP_Oj->box.w/2;
    NP_Oj->box.y = 0;

    CameraFollower *NP_UI_cmfl = new CameraFollower(*NP_Oj);
    NP_Oj->AddComponent((std::shared_ptr<CameraFollower>)NP_UI_cmfl);
    Game::GetInstance().GetCurrentState().AddObject(NP_Oj);
} 
   
void NP::Start() {     
    
}   
  
NP::~NP(){ 
    HideReader();
    if(NP_Oj != nullptr){
        NP_Oj->RequestDelete();
        NP_Oj = nullptr;
    }
} 
 
void NP::Update(float dt){  
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());
    if(NP_Oj->box.Contains(mousePos.x - Camera::pos.x * Game::resizer, mousePos.y- Camera::pos.y * Game::resizer)){
        ShowReader();
    } else {
        HideReader();
    }
}

void NP::Render(){ 
} 

void NP::ChangeNPLevel(){
    auto spriteComponent = NP_Oj->GetComponent("Sprite");
    auto spriteComponentPtr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
    if (spriteComponentPtr) {
        NP_Oj->RemoveComponent(spriteComponentPtr);
    }   

    auto CameraFollowerComponent = NP_Oj->GetComponent("CameraFollower");
    auto CameraFollowerComponentPtr = std::dynamic_pointer_cast<CameraFollower>(CameraFollowerComponent);
    if (CameraFollowerComponentPtr) {
        NP_Oj->RemoveComponent(CameraFollowerComponentPtr);
    }            

    std::string stringPath;
    if(GameData::npLevel == 0){
        stringPath = UI_NP0;
    }
    else if(GameData::npLevel == 1){
        stringPath = UI_NP1;
    }
    else if(GameData::npLevel == 2){
        stringPath = UI_NP2;
    }
    else{
        stringPath = UI_NP2;
    }

    Sprite* NP_spr = new Sprite(*NP_Oj, stringPath); 
    NP_spr->SetAlpha(225);
    NP_Oj->AddComponent((std::shared_ptr<Sprite>)NP_spr); 

    NP_spr->SetScale(0.75, 0.75);
    NP_Oj->box.x = RESOLUTION_WIDTH/2 - NP_Oj->box.w/2;
    NP_Oj->box.y = 0;

    CameraFollower *NP_UI_cmfl = new CameraFollower(*NP_Oj);
    NP_Oj->AddComponent((std::shared_ptr<CameraFollower>)NP_UI_cmfl);
}

void NP::ShowReader(){
    if (!reader) {
        reader = new GameObject(NP_Oj->box.x , NP_Oj->box.y);

        Reader* reader_behaviour = new Reader(*reader, MESSAGE_NP);
        reader->AddComponent(std::shared_ptr<Reader>(reader_behaviour));
        Game::GetInstance().GetCurrentState().AddObject(reader);

        reader->box.x = NP_Oj->box.x + NP_Oj->box.w/2 - reader->box.w/2;
        reader->box.y = NP_Oj->box.y + NP_Oj->box.h - reader->box.h/2;

    }
}

void NP::HideReader() {
    if (reader!= nullptr) {
        reader->RequestDelete();
        reader = nullptr;
    } 
}

bool NP::Is(std::string type){
    return (type == "NP");
}

