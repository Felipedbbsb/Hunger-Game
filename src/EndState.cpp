#include "EndState.h"
#include "Game.h"
#include "InputManager.h" 
#include "CameraFollower.h"
#include "Camera.h"
#include "GameData.h"
#include "Map.h"
#include "MenuState.h"


EndState::EndState() 
: State::State(),
pressBar(nullptr),
header(nullptr)
{

}
  
EndState::~EndState(){
    

    
}

void EndState::Update(float dt){   
    InputManager& input = InputManager::GetInstance();
 
 
    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }

    if (input.KeyPress(SPACEBAR_KEY)){
        MenuState* initialState = new MenuState();    
        Game::GetInstance().Push(initialState); 
        popRequested = true;
    }


    auto nextComponent = header->GetComponent("Sprite");
    auto nextComponentPtr = std::dynamic_pointer_cast<Sprite>(nextComponent);
    if(nextComponentPtr){
        if(nextComponentPtr->GetAlpha() != 255){
            nextComponentPtr->SetAlpha((nextComponentPtr->GetAlpha() + dt) * (APLHA_FADEOUT - 0.1 * (nextComponentPtr->GetAlpha() / 255) ));
            if(nextComponentPtr->GetAlpha() > 255){
                nextComponentPtr->SetAlpha(255);
            }
        }
        else{
            nextComponent = pressBar->GetComponent("Sprite");
            nextComponentPtr = std::dynamic_pointer_cast<Sprite>(nextComponent);
            if(nextComponentPtr->GetAlpha() != 255){
            nextComponentPtr->SetAlpha((nextComponentPtr->GetAlpha() + dt) * APLHA_FADEOUT);
            if(nextComponentPtr->GetAlpha() > 255){
                nextComponentPtr->SetAlpha(255);
            }
        }

        }
    } 
     

         

    UpdateArray(dt); 
    Camera::Update(dt);

    State::Update(dt);
} 

void EndState::LoadAssets(){
    //============================ Background ========================================

    GameObject *bg = new GameObject();
    Sprite* bgSprite= new Sprite(*bg, END_BG);
    CameraFollower *bg_cmfl = new CameraFollower(*bg);
    bg->AddComponent((std::shared_ptr<CameraFollower>)bg_cmfl);       
    bg->AddComponent((std::shared_ptr<Component>)bgSprite);

    bg->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - bg->box.w / 2;

    AddObject(bg);

    //============================ UI ========================================
    pressBar = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    Sprite* ui_behaviour = new Sprite(*pressBar, PRESS_SPACE_SPRITE); 
    pressBar->AddComponent((std::shared_ptr<Sprite>)ui_behaviour); 

    ui_behaviour->SetAlpha(0);
    pressBar->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - pressBar->box.w / 2;
    pressBar->box.y = (RESOLUTION_HEIGHT * Game::resizer) * 0.75;


    CameraFollower *ui_cmfl = new CameraFollower(*pressBar);
    pressBar->AddComponent((std::shared_ptr<CameraFollower>)ui_cmfl);
    AddObject(pressBar);

    //===================================================================
    header = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    Sprite* header_behaviour = new Sprite(*header, END_HEADER); 
    header->AddComponent((std::shared_ptr<Sprite>)header_behaviour); 

    header_behaviour->SetAlpha(0);
    header->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - header->box.w / 2;
    header->box.y = (RESOLUTION_HEIGHT * Game::resizer) * 0.35 - header->box.h / 2;


    CameraFollower *header_cmfl = new CameraFollower(*header);
    header->AddComponent((std::shared_ptr<CameraFollower>)header_cmfl);
    AddObject(header);
}






void EndState::Render(){     
    RenderArray();
    State::Render();
}

void EndState::Start(){
    LoadAssets();
    StartArray();
    started = true;

}
 
void EndState::Pause(){
    State::Pause();
}

void EndState::Resume(){
    State::Resume();

}

