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
#include "Protected.h"
#include "Mural.h"
#include "GameData.h"
#include "NP.h"
#include "EndState.h"
#include "Protected.h"
#include "Music.h"


bool CombatState::InteractionSCreenActivate = false;

std::vector<Enemies::EnemyId> CombatState::enemiesArrayIS = {};
 
Skill::AttackType CombatState::attackType = Skill::AttackType::NONE;

Skill::TargetType CombatState::whoAttacks = Skill::TargetType::IRR;

Skill::TargetType CombatState::whoReceives = Skill::TargetType::IRR; //if IRR probably palyer is attacking

bool CombatState::ChangingSides = false;

bool CombatState::motherTransition = false;

bool CombatState::popRequestedEndState = false;

CombatState::CombatState(std::vector<Enemies::EnemyId> enemiesArray, std::string spriteBackground) 
: State::State(),
enemiesArray(enemiesArray),
papiro(nullptr),
skillSelection(nullptr),
spriteBackground(spriteBackground),
toggleState(true),
toggleStateNP(true),
sandParticles(new SandParticles(Game::GetInstance().GetRenderer(), RESOLUTION_WIDTH, RESOLUTION_HEIGHT - RESOLUTION_HEIGHT * 1/3, 8, 35, 1, 3)){}
  
CombatState::~CombatState(){
    delete skillSelection;
    if(skillSelection != nullptr){
        skillSelection->RequestDelete();
        skillSelection = nullptr;
    }
    delete papiro;
    if(papiro != nullptr){
        papiro->RequestDelete(); 
        papiro = nullptr;
    }

    for (int i = Enemies::enemiesArray.size() - 1; i >= 0; i--) { //remove skills
            Enemies::enemiesArray.erase(Enemies::enemiesArray.begin() + i);
    }

    Enemies::enemiesArray.clear();

    delete sandParticles;     
}

void CombatState::Update(float dt){   

    if(CombatState::popRequestedEndState){
        CombatState::popRequestedEndState = false;
        CombatState::motherTransition = false;
        EndState* new_stage = new EndState(); 
        Game::GetInstance().Push(new_stage); 
        popRequested = true;
    }

    if(CombatState::motherTransition && toggleStateNP){
        State::FadeScreen(true, 0.0f);
        State::SetFadeTime(0.5f);
        toggleStateNP = false;
    }
    else if(!CombatState::motherTransition && !toggleStateNP){
        State::FadeScreen(false, 1.0f);
        State::SetFadeTime(0.5f);
        toggleStateNP = true;
    }

    InputManager& input = InputManager::GetInstance();
 
 
    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }
    if (input.KeyPress(SPACEBAR_KEY)){
        popRequested = true;
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
                    skillSelection = new GameObject();
                    new SkillSelection(*skillSelection, false);
                    AddObject(skillSelection);
                }       
            }

            if(SkillSelection::endSkillSelection){
                skillSelectionEnd.Update(dt);
                if(skillSelectionEnd.Get() >= SKILL_SELECTION_COOLDOWN_START){
                    popRequested = true;
                    skillSelectionEnd.Restart();
                    return; 
                }
            }
        }
        

        sandParticles->Update(dt);
        //SDL_RenderClear(renderer);
        

        UpdateArray(dt); 
        Camera::Update(dt);
        if(papiro != nullptr){
            papiro->RequestDelete();
            papiro = nullptr; 
        }
        
    }
    else{
        if(toggleState){
            toggleState = false;
        }

        // Handle Interaction Screen updates
        if(papiro == nullptr){
            // Create a new Papiro object for Interaction Screen
            papiro = new GameObject();
            new Papiro(*papiro, spriteBackground , CombatState::enemiesArrayIS,
                                                        CombatState::attackType,
                                                        CombatState::whoAttacks,
                                                        CombatState::whoReceives);


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
    new UI(*ui); 
    new CameraFollower(*ui);
    AddObject(ui);
    //PROTECTED
    GameObject* protected_UI = new GameObject(DAUGHTER_POS);
        new Protected(*protected_UI);
        protected_UI->box.x -= protected_UI->box.w/4;
        protected_UI->box.y -= protected_UI->box.h + 300;
        Game::GetInstance().GetCurrentState().AddObject(protected_UI);
     //NP
    GameObject* NP_UI = new GameObject(0, 0);
        new NP(*NP_UI);
        Game::GetInstance().GetCurrentState().AddObject(NP_UI);  
    Music combatMusic;
    combatMusic.Open("assets/audio/songCombat.mp3");
    combatMusic.Play();    
}   
 
void CombatState::CreateEnemies(){
    //============================ Enemies ========================================
    for (int i = enemiesArray.size() - 1; i >= 0; i--) {
        int offsetArray = enemiesArray.size() - i - 1;
        GameObject* enemy = new GameObject(ENEMIES_POS1.x + 200 * offsetArray, ENEMIES_POS1.y);
        // Acesse o Skill::SkillId a partir do std::shared_ptr<Skill>
        new Enemies(*enemy, enemiesArray[i]);
        std::weak_ptr<GameObject> weak_enemy = AddObject(enemy);
        Enemies::enemiesArray.push_back(weak_enemy);
    }
}

void CombatState::CreatePlayers(){
    //============================ Mother ========================================
    GameObject *mom = new GameObject(MOTHER_POS);
    new Mother(*mom);
    std::weak_ptr<GameObject> weak_mother = AddObject(mom);
    Mother::motherInstance = weak_mother;

    //============================ Daughter ========================================
    GameObject *daughter = new GameObject(DAUGHTER_POS);
    new Daughter(*daughter);
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
    new Sprite(*thirdPathObj, thirdPath);

    thirdPathObj->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - thirdPathObj->box.w / 2;

    new CameraParallax(*thirdPathObj, 0.75);

    AddObject(thirdPathObj);

    //================================== floorPathObj ==============================================
    GameObject *floorPathObj = new GameObject();
    new Sprite(*floorPathObj, floorPath);
   
    floorPathObj->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - floorPathObj->box.w / 2; 

    new CameraParallax(*floorPathObj, -0.10);

    
    AddObject(floorPathObj);
    //================================== secondPathObj ==============================================
    GameObject *secondPathObj = new GameObject();
    new Sprite(*secondPathObj, secondPath);
    
    secondPathObj->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - secondPathObj->box.w / 2;
    
    new CameraParallax(*secondPathObj, 0.5);
 
    AddObject(secondPathObj);


    //==================================
    //create combat components
    CreateEnemies();   
    CreatePlayers();
    //==================================

    //================================== firstPathObj ==============================================
    GameObject *firstPathObj = new GameObject();
    new Sprite(*firstPathObj, firstPath);

    firstPathObj->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - firstPathObj->box.w / 2;

    new CameraParallax(*firstPathObj, 0.25);

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
    sandParticles->Render();
    State::Render();
    
    
}

void CombatState::Start(){
    LoadAssets();
    StartArray();
    started = true;

    CombatState::InteractionSCreenActivate = false;
    CombatState::enemiesArrayIS.clear();
    CombatState::attackType = Skill::AttackType::NONE;
    CombatState::whoAttacks = Skill::TargetType::IRR;
    CombatState::whoReceives = Skill::TargetType::IRR;
    GameData::playerTurn = true;
    CombatState::ChangingSides = false;
    CombatState::popRequestedEndState = false;
    CombatState::motherTransition = false;

    Protected::isProtected = false;

    Enemies::SkillAllenemies = 0;//how many left enemies to receive skill effects

    Enemies::provokedEnemies = 0;//how many left enemies has provoke

    Enemies::enemiesToAttack = 0;//how many left enemies to attack

    Enemies::enemyAttacking = false;

    SkillSelection::skillSelectionActivated = false;
    SkillSelection::selectionSkillDjinnStyle = false;
    SkillSelection::endSkillSelection = false;

    Camera::Unfollow();
    Camera::pos.x = -FOCUS_ENEMY;
    Camera::pos.y = 0;

    sandParticles->toggleParticles();
    
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
    CombatState::popRequestedEndState = false;
    CombatState::motherTransition = false;
    
    
}

