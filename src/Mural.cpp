#include "Mural.h"
#include "Game.h"
#include "InputManager.h" 
#include "CameraFollower.h"
#include "Camera.h"
#include "Text.h"
#include "UI.h"
#include "Skill.h" 
#include "Papiro.h" 
#include "SkillSelection.h" 
#include "Protected.h"


bool Mural::MuralStateActivateReward = false;
bool Mural::MuralState = false;

Mural::Mural(std::string spriteBackground) 
: State::State(),
skillSelection(nullptr),
spriteBackground(spriteBackground)
{}
  
Mural::~Mural(){
    Mural::MuralStateActivateReward = false;
    Mural::MuralState = false;

    
}

void Mural::Update(float dt){   
    InputManager& input = InputManager::GetInstance();
 
 
    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }


    //============================ Checks whether to delete objects and updates ========================================

        if(Mural::MuralStateActivateReward){
            skillSelectionStart.Update(dt);
            if(skillSelectionStart.Get() >= 0){
                if(skillSelection == nullptr){
                    // Create a new skillSelection object for skillSelection Screen 
                    skillSelection = new GameObject();
                    SkillSelection* skillSelection_behaviour = new SkillSelection(*skillSelection, true);
                    skillSelection->AddComponent((std::shared_ptr<Component>)skillSelection_behaviour);
                    AddObject(skillSelection);
                } 
                
                if(SkillSelection::endSkillSelection){
                    skillSelectionEnd.Update(dt);
                    if(skillSelectionEnd.Get() >= SKILL_SELECTION_COOLDOWN_START){
                        popRequested = true;
                        if(skillSelection != nullptr){
                            skillSelection->RequestDelete();
                            
                        }

                    }
                }      
            }
        }
        

        UpdateArray(dt); 
        Camera::Update(dt);

    State::Update(dt);
} 

void Mural::LoadAssets(){
    //============================ Background ========================================

    CreateBackground(spriteBackground);

    //============================ UI ========================================
    //UI takes up 1/3 of the box at the bottom
    GameObject *ui = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    UI* ui_behaviour = new UI(*ui); 
    ui->AddComponent((std::shared_ptr<UI>)ui_behaviour); 
    CameraFollower *bg_cmfl = new CameraFollower(*ui);
    ui->AddComponent((std::shared_ptr<CameraFollower>)bg_cmfl);
    AddObject(ui);
}


void Mural::CreateBackground(std::string originalPath){     
    GameObject *bg = new GameObject();
    Sprite* bgSprite= new Sprite(*bg, originalPath);
    CameraFollower *bg_cmfl = new CameraFollower(*bg);
    bg->AddComponent((std::shared_ptr<CameraFollower>)bg_cmfl);       
    bg->AddComponent((std::shared_ptr<Component>)bgSprite);

    bg->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - bg->box.w / 2;

    AddObject(bg);
       
} 



void Mural::Render(){     
    RenderArray();
    State::Render();
}

void Mural::Start(){
    LoadAssets();
    StartArray();
    started = true;

    Mural::MuralStateActivateReward = false;
    Mural::MuralState = true;
    SkillSelection::endSkillSelection = false;


    Camera::Unfollow();
    Camera::pos.x = -FOCUS_ENEMY;
    Camera::pos.y = 0;
    

}
 
void Mural::Pause(){
    State::Pause();
}

void Mural::Resume(){
    State::Resume();

}

