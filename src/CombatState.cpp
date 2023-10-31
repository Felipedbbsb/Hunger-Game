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
#include "Papiro.h" 

bool CombatState::InteractionSCreenActivate = false;

std::vector<Enemies::EnemyId> CombatState::enemiesArrayIS = {};
 
Skill::AttackType CombatState::attackType = Skill::AttackType::NONE;

Skill::TargetType CombatState::whoAttacks = Skill::TargetType::IRR;

Skill::TargetType CombatState::whoReceives = Skill::TargetType::IRR; //if IRR probably palyer is attacking


CombatState::CombatState(std::vector<Enemies::EnemyId> enemiesArray, std::string spriteBackground) 
: State::State(),
enemiesArray(enemiesArray),
papiro(nullptr),
spriteBackground(spriteBackground){} 
  
CombatState::~CombatState(){}

void CombatState::Update(float dt){   
    InputManager& input = InputManager::GetInstance();

    Camera::Update(dt);
 
    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }

    //if (CombatState::InteractionSCreenActivate){
    //    InteractionState* new_stage = new InteractionState(CombatState::enemiesArrayIS,
    //                                                      CombatState::attackType,
   //                                                       CombatState::whoAttacks,
    //                                                      CombatState::whoReceives);
        //Game::GetInstance().Push(new_stage); 
        
    //}


    //============================ Checks whether to delete objects and updates ========================================
    if(!CombatState::InteractionSCreenActivate){
       UpdateArray(dt);
       papiro = nullptr; 
    }
    else{
        if(papiro == nullptr){
            papiro = new GameObject();
            Papiro* papiro_behaviour = new Papiro(*papiro, spriteBackground , CombatState::enemiesArrayIS,
                                                        CombatState::attackType,
                                                        CombatState::whoAttacks,
                                                        CombatState::whoReceives);
            papiro->AddComponent((std::shared_ptr<Component>)papiro_behaviour);
            AddObject(papiro);
        }
        else{
            papiro->Update(dt);
        }
    }
    
     
}

void CombatState::LoadAssets(){
    //============================ Background ========================================
    GameObject *bg = new GameObject();
    Sprite* bgSprite= new Sprite(*bg, spriteBackground);
    bg->AddComponent((std::shared_ptr<Component>)bgSprite);
    AddObject(bg);

    //============================ UI ========================================
    //UI takes up 1/3 of the box at the bottom
    GameObject *ui = new GameObject(0, RESOLUTION_HEIGHT * 2/3);

        //Adding skills
        std::vector<Skill::SkillId> skillArrayNormal;
        skillArrayNormal.push_back(Skill::Helmbreaker);
        skillArrayNormal.push_back(Skill::Rockabye);
        skillArrayNormal.push_back(Skill::Stinger);
        skillArrayNormal.push_back(Skill::HnS);
        
        std::vector<Skill::SkillId> skillArrayDjinn;
        skillArrayDjinn.push_back(Skill::InstantRegret);

 

    UI* ui_behaviour = new UI(*ui, skillArrayNormal, skillArrayDjinn); 
    ui->AddComponent((std::shared_ptr<UI>)ui_behaviour); 
    CameraFollower *bg_cmfl = new CameraFollower(*ui);
    ui->AddComponent((std::shared_ptr<CameraFollower>)bg_cmfl);
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
    CombatState::InteractionSCreenActivate = false;
    CombatState::enemiesArrayIS.clear();
    CombatState::attackType = Skill::AttackType::NONE;
    CombatState::whoAttacks = Skill::TargetType::IRR;
    CombatState::whoReceives = Skill::TargetType::IRR;
}

