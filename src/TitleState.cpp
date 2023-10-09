#include "TitleState.h"
#include "Game.h"
#include "InputManager.h" 
#include "CameraFollower.h"
#include "Camera.h"
#include "Text.h"
#include "UI.h"
#include "Mother.h"
#include "Daughter.h"
#include "Skill.h" 

TitleState::TitleState() : State::State(){
}

TitleState::~TitleState(){}

void TitleState::Update(float dt){   
    InputManager& input = InputManager::GetInstance();

    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }


    //============================ Checks whether to delete objects and updates ========================================
    UpdateArray(dt);
    
}

void TitleState::LoadAssets(){
    //============================ Background ========================================
    GameObject *bg = new GameObject();
    Sprite* menuSprite= new Sprite(*bg, TITLE_IMAGE);
    bg->AddComponent((std::shared_ptr<Component>)menuSprite);
    AddObject(bg);

    //============================ UI ========================================
    //UI takes up 1/3 of the box at the bottom
    GameObject *ui = new GameObject(0, RESOLUTION_HEIGHT * 2/3);

        //Adding skills
        std::vector<std::shared_ptr<Skill>> skillArray;
        skillArray.push_back(std::make_shared<Skill>(*ui, Skill::SKILL1));
        skillArray.push_back(std::make_shared<Skill>(*ui, Skill::SKILL2));
        skillArray.push_back(std::make_shared<Skill>(*ui, Skill::SKILL2));
        


    UI* ui_behaviour = new UI(*ui, skillArray);
    ui->AddComponent((std::shared_ptr<UI>)ui_behaviour);
    AddObject(ui);

    //============================ Mother ========================================
    GameObject *mom = new GameObject(600, RESOLUTION_HEIGHT * 2/3 - 375);
    Mother* mom_behaviour= new Mother(*mom);
    mom->AddComponent((std::shared_ptr<Mother>)mom_behaviour);
    AddObject(mom);

    //============================ Daughter ========================================
    GameObject *daughter = new GameObject(300, RESOLUTION_HEIGHT * 2/3 - 300);
    Daughter* daughter_behaviour= new Daughter(*daughter);
    daughter->AddComponent((std::shared_ptr<Daughter>)daughter_behaviour);
    AddObject(daughter);
}

void TitleState::Render(){
    RenderArray();
}

void TitleState::Start(){
    LoadAssets();
    StartArray();
    started = true;
}
 
void TitleState::Pause(){}

void TitleState::Resume(){

}