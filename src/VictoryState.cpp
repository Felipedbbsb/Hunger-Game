#include "VictoryState.h"
#include "Game.h"
#include "InputManager.h" 
#include "CameraFollower.h"
#include "Camera.h"
#include "GameData.h"
#include "Map.h"
#include "MenuState.h"


VictoryState::VictoryState() 
: State::State(),
pressBar(nullptr),
header(nullptr)
{

}
  
VictoryState::~VictoryState(){
    

    
} 

void VictoryState::Update(float dt){   
    InputManager& input = InputManager::GetInstance();
 
 
    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }

    if (input.KeyPress(SPACEBAR_KEY)){
        MenuState* initialState = new MenuState();    
        Game::GetInstance().Push(initialState); 
        popRequested = true;
        popRequestAll = true;

        GameObject* selectedSFX = new GameObject();
        Sound *selectSFX_sound = new Sound(*selectedSFX, SKILL_SELECTION_CONFIRMED); 
         selectSFX_sound->Play(1);
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

void VictoryState::LoadAssets(){
    //============================ Background ========================================

    GameObject *bg = new GameObject();
    new Sprite(*bg, VICTORY_BG);
    new CameraFollower(*bg);

    bg->box.x = RESOLUTION_WIDTH  / 2 - bg->box.w / 2;

    AddObject(bg);

    //============================ UI ========================================
    pressBar = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    Sprite *ui_behaviour = new Sprite(*pressBar, PRESS_SPACE_SPRITE); 

    ui_behaviour->SetAlpha(0);
    pressBar->box.x = RESOLUTION_WIDTH  / 2 - pressBar->box.w / 2;
    pressBar->box.y = (RESOLUTION_HEIGHT ) * 0.75;


    new CameraFollower(*pressBar);

    AddObject(pressBar);

    //===================================================================
    header = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    Sprite *header_behaviour = new Sprite(*header, VICTORY_HEADER); 

    header_behaviour->SetAlpha(0);
    header->box.x = RESOLUTION_WIDTH  / 2 - header->box.w / 2;
    header->box.y = (RESOLUTION_HEIGHT ) * 0.35 - header->box.h / 2;


    new CameraFollower(*header);

    AddObject(header);

    Music noncombatMusic;
    noncombatMusic.Open("assets/audio/songNonCombat.mp3");
    noncombatMusic.Play();
}






void VictoryState::Render(){     
    RenderArray();
    State::Render();
}

void VictoryState::Start(){
    LoadAssets();
    StartArray();
    started = true;

}
 
void VictoryState::Pause(){
    State::Pause();
}

void VictoryState::Resume(){
    State::Resume();

}

