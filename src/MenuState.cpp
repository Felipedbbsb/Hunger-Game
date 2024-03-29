#include "MenuState.h"
#include "Game.h"
#include "InputManager.h" 
#include "CameraFollower.h"
#include "Camera.h"
#include "GameData.h"
#include "Map.h"
#include "Mother.h"
#include "Daughter.h"
#include "Cutscene.h"


MenuState::MenuState() 
: State::State(),
pressBar(nullptr),
header(nullptr)
{

}
   
MenuState::~MenuState(){

    delete pressBar;
    delete header;
    
}

void MenuState::Update(float dt){   
    InputManager& input = InputManager::GetInstance();
 
 
    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }

    if (input.KeyPress(SPACEBAR_KEY)){

        //Resets Data

          Skill::InitializeSkills();

          GameData::playerTurn = true; // victory condition 
          GameData::hpCorrupted = 0; 
          GameData::hp = MOTHER_HP - GameData::hpCorrupted;  
          GameData::hpMax = MOTHER_HP; 
          GameData::life = DAUGHTER_HP; 
          GameData::lifeMax = DAUGHTER_HP;  
          GameData::npLevel = 0; //init 
         

        Cutscene* initialState = new Cutscene();    
        Game::GetInstance().Push(initialState); 
        popRequested = true;

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

void MenuState::LoadAssets(){
    //============================ Background ========================================

    GameObject *bg = new GameObject();
    new Sprite(*bg, MENU_BG);
    new CameraFollower(*bg);


    bg->box.x = RESOLUTION_WIDTH  / 2 - bg->box.w / 2;

    AddObject(bg);

    //============================ UI ========================================
    pressBar = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    Sprite* ui_behaviour = new Sprite(*pressBar, PRESS_SPACE_SPRITE); 


    ui_behaviour->SetAlpha(0);
    pressBar->box.x = RESOLUTION_WIDTH / 2 - pressBar->box.w / 2;
    pressBar->box.y = (RESOLUTION_HEIGHT ) * 0.75;


    new CameraFollower(*pressBar);
    AddObject(pressBar);

    //===================================================================
    header = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    Sprite* header_behaviour = new Sprite(*header, HEADER_MENU_BG); 

    header_behaviour->SetAlpha(0);
    header->box.x = RESOLUTION_WIDTH   / 2 - header->box.w / 2;
    header->box.y = (RESOLUTION_HEIGHT ) * 0.35 - header->box.h / 2;

    Sound *header_sound = new Sound(*header, "assets/audio/sfxGameStart.mp3");

    header_sound->Play(1);

    new CameraFollower(*header);

    AddObject(header);


    Music noncombatMusic;
    noncombatMusic.Open("assets/audio/songNonCombat.mp3");
    noncombatMusic.Play();  
}






void MenuState::Render(){     
    RenderArray();
    State::Render();
}

void MenuState::Start(){
    LoadAssets();
    StartArray();
    started = true;

}
 
void MenuState::Pause(){
    State::Pause();
}

void MenuState::Resume(){
    State::Resume();

}

