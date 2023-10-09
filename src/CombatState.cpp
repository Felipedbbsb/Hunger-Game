#include "CombatState.h"
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

CombatState::CombatState() : State::State(){
} 

CombatState::~CombatState(){}

void CombatState::Update(float dt){   
    InputManager& input = InputManager::GetInstance();

    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }


    //============================ Checks whether to delete objects and updates ========================================
    UpdateArray(dt);
    
}

void CombatState::LoadAssets(){
    //============================ Background ========================================
    GameObject *bg = new GameObject();
    Sprite* menuSprite= new Sprite(*bg, COMBAT_IMAGE);
    bg->AddComponent((std::shared_ptr<Component>)menuSprite);
    AddObject(bg);

    //============================ UI ========================================
    //UI takes up 1/3 of the box at the bottom
    GameObject *ui = new GameObject(0, RESOLUTION_HEIGHT * 2/3);

        //Adding skills
        std::vector<std::shared_ptr<Skill>> skillArrayNormal;
        skillArrayNormal.push_back(std::make_shared<Skill>(*ui, Skill::SKILL2));
        skillArrayNormal.push_back(std::make_shared<Skill>(*ui, Skill::SKILL4));
        skillArrayNormal.push_back(std::make_shared<Skill>(*ui, Skill::SKILL2));
        
        std::vector<std::shared_ptr<Skill>> skillArrayDjinn;
        skillArrayDjinn.push_back(std::make_shared<Skill>(*ui, Skill::SKILL1));
        skillArrayDjinn.push_back(std::make_shared<Skill>(*ui, Skill::SKILL3));



    UI* ui_behaviour = new UI(*ui, skillArrayNormal, skillArrayDjinn); 
    ui->AddComponent((std::shared_ptr<UI>)ui_behaviour);
    AddObject(ui);

    //============================ Mother ========================================
    GameObject *mom = new GameObject(MOTHER_POS);
    Mother* mom_behaviour= new Mother(*mom);
    mom->AddComponent((std::shared_ptr<Mother>)mom_behaviour);
    AddObject(mom);

    //============================ Daughter ========================================
    GameObject *daughter = new GameObject(DAUGHTER_POS);
    Daughter* daughter_behaviour= new Daughter(*daughter);
    daughter->AddComponent((std::shared_ptr<Daughter>)daughter_behaviour);
    AddObject(daughter);

    //============================ Enemies ========================================
    GameObject *enemy1 = new GameObject(ENEMIES_POS1);
    Enemies* enemy1_behaviour= new Enemies(*enemy1);
    enemy1->AddComponent((std::shared_ptr<Enemies>)enemy1_behaviour); 
    AddObject(enemy1);

    GameObject *enemy2 = new GameObject(ENEMIES_POS1.x + 300, ENEMIES_POS1.y);
    Enemies* enemy2_behaviour= new Enemies(*enemy2);
    enemy2->AddComponent((std::shared_ptr<Enemies>)enemy2_behaviour); 
    AddObject(enemy2);
}

void CombatState::Render(){
    RenderArray();
}

void CombatState::Start(){
    LoadAssets();
    StartArray();
    started = true;
}
 
void CombatState::Pause(){}

void CombatState::Resume(){

}