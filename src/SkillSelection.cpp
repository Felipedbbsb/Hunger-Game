#include "SkillSelection.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Reader.h"
#include "GameData.h"
#include "CombatState.h"
#include "InteractionObject.h"
#include "CameraFollower.h"
#include "Skill.h"
#include "UI.h"

#include <algorithm>

bool SkillSelection::skillSelectionActivated = false;
bool SkillSelection::selectionSkillDjinnStyle = false;
bool SkillSelection::endSkillSelection = false;

SkillSelection::SkillSelection(GameObject &associated,  bool isDjinn)
    : Component(associated),
    background(nullptr), 
    passButon(nullptr),
    toggleArrow(false),
    objectsMoves(0),
    rewardArrayObj({})
    {
        SkillSelection::skillSelectionActivated = true;

        Skill::selectedSkill = nullptr; //generic 

        Skill::selectedSkillEnemy = nullptr; //generic  

        Skill::skillBackToMother = nullptr; //back effects

        Skill::skillBackToDaughter = nullptr; //back effects 

        SkillSelection::selectionSkillDjinnStyle = isDjinn;
}
  
void SkillSelection::Start() {

    //background
    CreateBackground();
    

    //pass button
    

    CreatePassButon();

    //Skills to be selected/changed/ignored
    CreateSkillOptions();

    SkillSelection::endSkillSelection = false;
    GameData::playerTurn = true;
    
    GameObject* focusCamera =  new GameObject(-FOCUS_ENEMY, 0);
    Camera::Follow(focusCamera);
    CombatState::ChangingSides = true;
    
}   



SkillSelection::~SkillSelection() {
    SkillSelection::skillSelectionActivated = false;
    SkillSelection::selectionSkillDjinnStyle = false;

    if(passButon != nullptr){
        passButon->RequestDelete();
        passButon = nullptr;
    }
    if(background != nullptr){
        background->RequestDelete();
        background = nullptr;
    }

    for (int i = rewardArrayObj.size() - 1; i >= 0; i--) { //remove skills
            rewardArrayObj.erase(rewardArrayObj.begin() + i);
    }

}

void SkillSelection::CreateBackground() {
    background = new GameObject();

    std::string bg_type;
    if(SkillSelection::selectionSkillDjinnStyle){
        bg_type = SKILL_SELECTION_MURAL_BCKGROUND;
    }
    else{
        bg_type = SKILL_SELECTION_BCKGROUND;
    }

    new Sprite(*background, bg_type); 
    
    background->box.x = (RESOLUTION_WIDTH )/2 - background->box.w/2 - Camera::pos.x;   
    background->box.y = - Camera::pos.y; 


    
    Game::GetInstance().GetCurrentState().AddObject(background); 
}

void SkillSelection::CreatePassButon() {
    passButon = new GameObject(0, 0);
    new Sprite(*passButon, SKILL_SELECTION_ARROW); 

    passButon->box.x =  background->box.x + (background->box.w ) - (passButon->box.w * SCALE_ARROW) - Camera::pos.x - OFFSET_SKILL_SELECTION_ARROW.x;   
    passButon->box.y = background->box.y + background->box.h - (passButon->box.h * SCALE_ARROW)  - Camera::pos.y - OFFSET_SKILL_SELECTION_ARROW.y; 
 
    Game::GetInstance().GetCurrentState().AddObject(passButon); 
} 
  
void SkillSelection::CreateSkillOptions() {
    // Get all skill IDs from Skill::skillInfoMap up to LOCKED1 (exclusive)
    auto itBegin = Skill::skillInfoMap.find(Skill::InstantRegret);
    //auto itEnd = Skill::skillInfoMap.end();
    auto itEnd = Skill::skillInfoMap.find(Skill::LOCKED1); 

    if (!selectionSkillDjinnStyle ) { 
        // If isDjinn is true, exclude skills that are before InstantRegret
        
        itBegin = Skill::skillInfoMap.begin();
        itEnd = Skill::skillInfoMap.find(Skill::InstantRegret);
    } 

    std::vector<Skill::SkillId> validSkillIds;
    for (auto it = itBegin; it != itEnd; ++it) { 
        validSkillIds.push_back(it->first); 
    }

    // Remove skills that are already present in Skill::skillArray
    for (auto skillId : Skill::skillArray) {
        auto it = std::find(validSkillIds.begin(), validSkillIds.end(), skillId);
        if (it != validSkillIds.end()) {
            validSkillIds.erase(it);
        }
    }

    // If there are not enough remaining skills, do something (e.g., throw an exception)
    if (validSkillIds.size() < 3) {
        // Handle the case where there are not enough skills
        // You may want to throw an exception or handle it differently based on your requirements
        // For now, I'll just return without creating any new skills
        return;
    }

    // Shuffle the valid skill IDs
    std::random_shuffle(validSkillIds.begin(), validSkillIds.end());

    // Select the first three shuffled skill IDs
    std::vector<Skill::SkillId> randomSkillIds;
    randomSkillIds.push_back(validSkillIds[0]);
    randomSkillIds.push_back(validSkillIds[1]);
    randomSkillIds.push_back(validSkillIds[2]);

    // Create GameObjects for each selected skill
    for (int i = 0; i < 3; ++i) {
        Skill::SkillId skillId = randomSkillIds[i];

        // Create a GameObject for the skill
        GameObject *skillObject = new GameObject();
        new Skill(*skillObject, skillId, UI::ap_behaviour, false);

        // Set the position and add to the current state
        skillObject->box.x = background->box.x + background->box.w / 2 - skillObject->box.w / 2 - OFFSET_SKILL_OPTIONS  + OFFSET_SKILL_OPTIONS * i;
        skillObject->box.y = background->box.y + OFFSET_SKILL_OPTIONSY;

        std::weak_ptr<GameObject> weak_reward = Game::GetInstance().GetCurrentState().AddObject(skillObject);

        rewardArrayObj.push_back(weak_reward);
    }
}


void SkillSelection::Update(float dt) {
    
    if(SkillSelection::endSkillSelection){
        objectsMoves += objectsMoves_VELOCITY * dt; 
    }
    
    background->box.x = objectsMoves + (RESOLUTION_WIDTH )/2 - background->box.w/2 - Camera::pos.x; 
    passButon->box.x =  background->box.x + background->box.w  - OFFSET_SKILL_SELECTION_ARROW.x - passButon->box.w/2;
   

    for(int i = rewardArrayObj.size() - 1; i >= 0; i-- ){
        rewardArrayObj[i].lock()->box.x =  background->box.x + background->box.w / 2 - rewardArrayObj[i].lock()->box.w / 2 - OFFSET_SKILL_OPTIONS  + OFFSET_SKILL_OPTIONS * i;
        rewardArrayObj[i].lock()->box.y = background->box.y + OFFSET_SKILL_OPTIONSY;  

        auto spriteComponent = rewardArrayObj[i].lock()->GetComponent("Sprite");
        auto spriteComponentPtr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
        if (spriteComponentPtr) {
            // Apply the desaturation effect
            spriteComponentPtr->SetDesaturation(true);
        }    
    }


    if (Skill::skillFromReward != nullptr) {
        // Encontra um Skill com o ID SkillId::EMPTY em Skill::skillArrayObj
        auto emptySkill = std::find_if(
            Skill::skillArrayObj.begin(),
            Skill::skillArrayObj.end(),
            [](const std::weak_ptr<GameObject>& weakSkill) {
                if (auto skillObject = weakSkill.lock()) {
                    if (auto skillComponent = skillObject->GetComponent("Skill")) {
                        auto skill = std::dynamic_pointer_cast<Skill>(skillComponent);
                        return skill && skill->GetId() == Skill::SkillId::EMPTY;
                    }
                }
                return false;
            }
        );

        // Se encontrar um Skill com o ID SkillId::EMPTY, define Skill::skillToReward como esse Skill
        if (emptySkill != Skill::skillArrayObj.end()) {
            if (auto skillObject = emptySkill->lock()) {
                if (auto skillComponent = skillObject->GetComponent("Skill")) {
                    Skill::skillToReward = std::dynamic_pointer_cast<Skill>(skillComponent).get();
                }
            }
        }
    }

    if(Skill::skillFromReward != nullptr && Skill::skillToReward != nullptr && !toggleArrow){
        toggleArrow = true;
        
        if(passButon != nullptr){
            auto arrowComponent = passButon->GetComponent("Sprite");
            auto arrowComponentPtr = std::dynamic_pointer_cast<Sprite>(arrowComponent);
            if (arrowComponentPtr) {
                passButon->RemoveComponent(arrowComponentPtr.get());
                new Sprite(*passButon, SKILL_SELECTION_ARROW_ACTIVATED); 
            } 
        }       
    }

    else if((Skill::skillFromReward == nullptr || Skill::skillToReward == nullptr) && toggleArrow){
        toggleArrow = false;

        if(passButon != nullptr){
            auto arrowComponent = passButon->GetComponent("Sprite"); 
            auto arrowComponentPtr = std::dynamic_pointer_cast<Sprite>(arrowComponent);
            if (arrowComponentPtr) {
                passButon->RemoveComponent(arrowComponentPtr.get());
                new Sprite(*passButon, SKILL_SELECTION_ARROW); 

            }
        }    
    }

    Vec2 mousePos(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());
    


    if (passButon->box.Contains(mousePos.x- Camera::pos.x, mousePos.y- Camera::pos.y)) {
        if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) && !SkillSelection::endSkillSelection){
            if(Skill::skillFromReward != nullptr && Skill::skillToReward != nullptr){
                auto skillFrom = Skill::skillFromReward->GetId();
                auto skillTo = Skill::skillToReward->GetId();
                Skill::AddSkill(skillFrom, skillTo);
                Skill::skillFromReward = nullptr;
                Skill::skillToReward = nullptr;
                SkillSelection::endSkillSelection = true;

            }
        }

        if(passButon != nullptr){
            auto arrowComponent = passButon->GetComponent("Sprite"); 
            auto arrowComponentPtr = std::dynamic_pointer_cast<Sprite>(arrowComponent);
               
               
               
               
                // Center position original
                auto posXenterX = passButon->box.x + passButon->box.w / 2;
                auto posXenterY = passButon->box.y + passButon->box.h / 2;

                arrowComponentPtr->SetScale(SCALE_ARROW, SCALE_ARROW);


                

                // Calcula a nova largura e altura
                passButon->box.x = posXenterX - passButon->box.w / 2;
                passButon->box.y = posXenterY - passButon->box.h / 2;
        }        
       
    }else{


        if(passButon != nullptr){
            auto arrowComponent = passButon->GetComponent("Sprite"); 
            auto arrowComponentPtr = std::dynamic_pointer_cast<Sprite>(arrowComponent);
                // Center position original
                auto posXenterX = passButon->box.x + passButon->box.w / 2;
                auto posXenterY = passButon->box.y + passButon->box.h / 2;

                // Call SetScale with the correct number of arguments
                arrowComponentPtr->SetScale(1.0, 1.0);

                // Postion correction
                passButon->box.x = posXenterX - passButon->box.w / 2;
                passButon->box.y = posXenterY - passButon->box.h / 2;
        }
    }  

    

}

void SkillSelection::Render() {
}
 
bool SkillSelection::Is(std::string type) { 
    return (type == "SkillSelection");
}
