#include "Cutscene.h"
#include "Game.h"
#include "InputManager.h" 
#include "CameraFollower.h"
#include "Camera.h"
#include "GameData.h"
#include "Map.h"
#include "Mother.h"
#include "Daughter.h"
#include "CameraShake.h"


Cutscene::Cutscene() 
: State::State(),
frameCut(nullptr),
frameCutCount(1),
frameTransition(false)
{

}
   
Cutscene::~Cutscene(){
    if(frameCut != nullptr){
        frameCut->RequestDelete();
        frameCut = nullptr;
    }
    delete frameCut;

    
}

void Cutscene::Update(float dt){   
    InputManager& input = InputManager::GetInstance();
 
 
    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }

    if (input.KeyPress(SPACEBAR_KEY)){

        //Resets Data

        Map* initialState = new Map();    
        Game::GetInstance().Push(initialState); 
        popRequested = true;

        GameObject* selectedSFX = new GameObject();
        Sound *selectSFX_sound = new Sound(*selectedSFX, SKILL_SELECTION_CONFIRMED); 
         selectSFX_sound->Play(1);
    }


    frameCutTime.Update(dt);
    if(frameCutTime.Get() >= TIME_PER_FRAMECUT && !frameTransition){
        State::FadeScreen(true, 0.0f);
        State::SetFadeTime(TIME_PER_FADEOUT);
        frameCutTime.Restart();
        frameTransition = true;

        if(frameCutCount >= CUTS_MAX){
            Map* initialState = new Map();    
            Game::GetInstance().Push(initialState); 
            popRequested = true;
            return;
        }
 

    }
    else if(frameCutTime.Get() >= TIME_PER_FADEOUT && frameTransition){
        ChangeFrameCut(); 

        State::FadeScreen(false, 1.0f);
        State::SetFadeTime(TIME_PER_FADEOUT);
        frameCutTime.Restart(); 
        frameTransition = false;   
    }     

    

    UpdateArray(dt); 
    Camera::Update(dt);

    State::Update(dt);
} 

void Cutscene::LoadAssets(){
    //============================ Background ========================================

    frameCut = new GameObject();
    new Sprite(*frameCut, GetFrameSprite());

    frameCut->box.x = RESOLUTION_WIDTH  / 2 - frameCut->box.w / 2;

    new CameraFollower(*frameCut);
    

    

    AddObject(frameCut);


    Music noncombatMusic;
    noncombatMusic.Open("assets/audio/songNonCombat.mp3");
    noncombatMusic.Play();  
}


void Cutscene::ChangeFrameCut(){
    frameCutCount++;


    auto nextComponent = frameCut->GetComponent("Sprite");
    auto nextComponentPtr = std::dynamic_pointer_cast<Sprite>(nextComponent);
    if(nextComponentPtr){
        frameCut->RemoveComponent(nextComponentPtr.get());
        new Sprite(*frameCut, GetFrameSprite());
    } 

}

std::string Cutscene::GetFrameSprite(){
    std::string fileName = std::string(PATH_CUTSCENES) + "/cut" + std::to_string(frameCutCount) + ".png";
    return fileName;
}

void Cutscene::Render(){     
    RenderArray();
    State::Render();
}

void Cutscene::Start(){
    LoadAssets();
    StartArray();
    started = true;

}
 
void Cutscene::Pause(){
    State::Pause();
}

void Cutscene::Resume(){
    State::Resume();

}

