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

CombatState::CombatState(std::vector<Enemies::EnemyId> enemiesArray) 
: State::State(),
enemiesArray(enemiesArray){
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
    for (int i = enemiesArray.size() - 1; i >= 0; i--) {
        int offsetArray = enemiesArray.size() - i - 1;
        GameObject* enemy = new GameObject(ENEMIES_POS1.x + 200 * offsetArray, ENEMIES_POS1.y);
        // Acesse o Skill::SkillId a partir do std::shared_ptr<Skill>
        Enemies* enemy_behaviour = new Enemies(*enemy, enemiesArray[i]);
        enemy->AddComponent(std::shared_ptr<Enemies>(enemy_behaviour));
        Game::GetInstance().GetCurrentState().AddObject(enemy);
    }

 
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