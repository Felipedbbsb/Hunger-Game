#include "InteractionState.h"
#include "Game.h"
#include "InputManager.h" 
#include "CameraFollower.h"
#include "Camera.h"
#include "Text.h"
#include "UI.h"
#include "Mother.h"
#include "Daughter.h"
#include "Enemies.h"
#include "Skill.h" 
#include "CombatState.h"
#include "GameData.h"

InteractionState::InteractionState(std::vector<Enemies::EnemyId> enemiesArrayIS, Skill::AttackType attackType,Skill::TargetType whoAttacks, Skill::TargetType whoReceives) 
: State::State(), interactionTime(){
} 
  
InteractionState::~InteractionState(){}

void InteractionState::Update(float dt){   
    InputManager& input = InputManager::GetInstance(); 

    Camera::Update(dt);

    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }


    interactionTime.Update(dt);
    if(interactionTime.Get() >= INTERECTION_COOLDOWN){
        interactionTime.Restart();
        popRequested = true; 
    } 

    //============================ Checks whether to delete objects and updates ========================================
    UpdateArray(dt);
    
}

void InteractionState::LoadAssets(){
    //============================ Background ========================================
    GameObject *bg = new GameObject();
    Sprite* menuSprite= new Sprite(*bg, PAPIRO_SPRITE);
    bg->AddComponent((std::shared_ptr<Component>)menuSprite);
    AddObject(bg);


    if(GameData::playerTurn == true){

    }
    else{
            
    }

    
}

void InteractionState::Render(){     
    RenderArray();
}

void InteractionState::Start(){
    LoadAssets();
    StartArray();
    started = true;

    CombatState::InteractionSCreenActivate = false;
}
 
void InteractionState::Pause(){}

void InteractionState::Resume(){

}

