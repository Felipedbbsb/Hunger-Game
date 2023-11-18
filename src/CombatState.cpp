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
#include "SkillSelection.h" 
#include "CameraParallax.h"

bool CombatState::InteractionSCreenActivate = false;

std::vector<Enemies::EnemyId> CombatState::enemiesArrayIS = {};
 
Skill::AttackType CombatState::attackType = Skill::AttackType::NONE;

Skill::TargetType CombatState::whoAttacks = Skill::TargetType::IRR;

Skill::TargetType CombatState::whoReceives = Skill::TargetType::IRR; //if IRR probably palyer is attacking

bool CombatState::ChangingSides = false;

CombatState::CombatState(std::vector<Enemies::EnemyId> enemiesArray, std::string spriteBackground) 
: State::State(),
enemiesArray(enemiesArray),
papiro(nullptr),
skillSelection(nullptr),
spriteBackground(spriteBackground),
toggleState(true){}
  
CombatState::~CombatState(){}

void CombatState::Update(float dt){   
    InputManager& input = InputManager::GetInstance();
 
 
    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }


    //============================ Checks whether to delete objects and updates ========================================
    

    if(!CombatState::InteractionSCreenActivate){
        if(!toggleState){// Checks with needs to resume the state
            toggleState = true;
        }
        // other Conditional for pausing the game
        if(Enemies::enemiesCount == 0){
            skillSelectionStart.Update(dt);
            if(skillSelectionStart.Get() >= SKILL_SELECTION_COOLDOWN_START){
                if(skillSelection == nullptr){
                    // Create a new skillSelection object for skillSelection Screen 
                    skillSelection = new GameObject();
                    SkillSelection* skillSelection_behaviour = new SkillSelection(*skillSelection, false);
                    skillSelection->AddComponent((std::shared_ptr<Component>)skillSelection_behaviour);
                    AddObject(skillSelection);
                }       
            }
        }
        

        UpdateArray(dt); 
        Camera::Update(dt);
        papiro = nullptr; 
    }
    else{
        if(toggleState){
            toggleState = false;
        }

        // Handle Interaction Screen updates
        if(papiro == nullptr){
            // Create a new Papiro object for Interaction Screen
            papiro = new GameObject();
            Papiro* papiro_behaviour = new Papiro(*papiro, spriteBackground , CombatState::enemiesArrayIS,
                                                        CombatState::attackType,
                                                        CombatState::whoAttacks,
                                                        CombatState::whoReceives);

            //CameraFollower *papiro_cmfl = new CameraFollower(*papiro);
            //papiro->AddComponent((std::shared_ptr<CameraFollower>)papiro_cmfl);
            papiro->AddComponent((std::shared_ptr<Component>)papiro_behaviour);
            AddObject(papiro);
        }
        else{ 
            // Update the existing Papiro object for Interaction Screen
            papiro->Update(dt);  
        }
    }

    State::Update(dt);
} 

void CombatState::LoadAssets(){
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

void CombatState::CreateEnemies(){
    //============================ Enemies ========================================
    for (int i = enemiesArray.size() - 1; i >= 0; i--) {
        int offsetArray = enemiesArray.size() - i - 1;
        GameObject* enemy = new GameObject(ENEMIES_POS1.x + 200 * offsetArray, ENEMIES_POS1.y);
        // Acesse o Skill::SkillId a partir do std::shared_ptr<Skill>
        Enemies* enemy_behaviour = new Enemies(*enemy, enemiesArray[i]);
        enemy->AddComponent(std::shared_ptr<Enemies>(enemy_behaviour));
        std::weak_ptr<GameObject> weak_enemy = AddObject(enemy);
        Enemies::enemiesArray.push_back(weak_enemy);
    }
}

void CombatState::CreatePlayers(){
    //============================ Mother ========================================
    GameObject *mom = new GameObject(MOTHER_POS);
    Mother* mom_behaviour= new Mother(*mom);
    mom->AddComponent((std::shared_ptr<Mother>)mom_behaviour);
    std::weak_ptr<GameObject> weak_mother = AddObject(mom);
    Mother::motherInstance = weak_mother;

    //============================ Daughter ========================================
    GameObject *daughter = new GameObject(DAUGHTER_POS);
    Daughter* daughter_behaviour= new Daughter(*daughter);
    daughter->AddComponent((std::shared_ptr<Daughter>)daughter_behaviour);
    std::weak_ptr<GameObject> weak_daughter = AddObject(daughter);
    Daughter::daughterInstance = weak_daughter;
}

//with enemies
void CombatState::CreateBackground(std::string originalPath){     
    
    std::string floorPath = GeneratePath(originalPath, "floor");
    std::string firstPath = GeneratePath(originalPath, "first");
    std::string secondPath = GeneratePath(originalPath, "second");
    std::string thirdPath = GeneratePath(originalPath, "third");


    //================================== thirdPathObj ==============================================
    GameObject *thirdPathObj = new GameObject();
    Sprite* thirdPathSprite= new Sprite(*thirdPathObj, thirdPath);
    thirdPathObj->AddComponent((std::shared_ptr<Component>)thirdPathSprite);

    thirdPathObj->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - thirdPathObj->box.w / 2;

    CameraParallax *thirdPathObj_cmfl = new CameraParallax(*thirdPathObj, 0.75);
    thirdPathObj->AddComponent((std::shared_ptr<CameraParallax>)thirdPathObj_cmfl);
    AddObject(thirdPathObj);

    //================================== floorPathObj ==============================================
    GameObject *floorPathObj = new GameObject();
    Sprite* floorPathSprite= new Sprite(*floorPathObj, floorPath);
    floorPathObj->AddComponent((std::shared_ptr<Component>)floorPathSprite);

    floorPathObj->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - floorPathObj->box.w / 2; 

    CameraParallax *floorPathObj_cmfl = new CameraParallax(*floorPathObj, -0.10);
    floorPathObj->AddComponent((std::shared_ptr<CameraParallax>)floorPathObj_cmfl);
    
    AddObject(floorPathObj);
    //================================== secondPathObj ==============================================
    GameObject *secondPathObj = new GameObject();
    Sprite* secondPathSprite= new Sprite(*secondPathObj, secondPath);
    secondPathObj->AddComponent((std::shared_ptr<Component>)secondPathSprite);
    secondPathObj->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - secondPathObj->box.w / 2;
    
    CameraParallax *secondPathObj_cmfl = new CameraParallax(*secondPathObj, 0.5);
    secondPathObj->AddComponent((std::shared_ptr<CameraParallax>)secondPathObj_cmfl);

    AddObject(secondPathObj);

    //==================================
    //create combat components
    CreateEnemies();   
    CreatePlayers();
    //==================================


    //================================== firstPathObj ==============================================
    GameObject *firstPathObj = new GameObject();
    Sprite* firstPathSprite= new Sprite(*firstPathObj, firstPath);
    firstPathObj->AddComponent((std::shared_ptr<Component>)firstPathSprite);

    firstPathObj->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - firstPathObj->box.w / 2;

    CameraParallax *firstPathObj_cmfl = new CameraParallax(*firstPathObj, 0.25);
    firstPathObj->AddComponent((std::shared_ptr<CameraParallax>)firstPathObj_cmfl);

    AddObject(firstPathObj);
 
    GameObject* focusCamera =  new GameObject(-FOCUS_ENEMY, 0);
            Camera::Follow(focusCamera);
            CombatState::ChangingSides = true;
       
} 

std::string CombatState::GeneratePath(std::string originalPath, std::string suffix) {
 
    size_t lastSlashPos = originalPath.find_last_of('/');

    std::string fileName = originalPath.substr(lastSlashPos + 1); 

    std::string path = originalPath.substr(0, lastSlashPos + 1);

    return path + suffix + ".png";
}


void CombatState::Render(){     
    RenderArray();
    State::Render();
}

void CombatState::Start(){
    LoadAssets();
    StartArray();
    started = true;

}
 
void CombatState::Pause(){
    State::Pause();
}

void CombatState::Resume(){
    State::Resume();

    CombatState::InteractionSCreenActivate = false;
    CombatState::enemiesArrayIS.clear();
    CombatState::attackType = Skill::AttackType::NONE;
    CombatState::whoAttacks = Skill::TargetType::IRR;
    CombatState::whoReceives = Skill::TargetType::IRR;


}

