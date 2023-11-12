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

#include <algorithm>

bool SkillSelection::skillSelectionActivated = false;
bool SkillSelection::selectionSkillDjinnStyle = false;

SkillSelection::SkillSelection(GameObject &associated,  bool isDjinn)
    : Component(associated),
    background(nullptr), 
    passButon(nullptr),
    toggleArrow(false)
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


}   



SkillSelection::~SkillSelection() {
    
}

void SkillSelection::CreateBackground() {
    background = new GameObject();
    Sprite* background_spr = new Sprite(*background, SKILL_SELECTION_BCKGROUND); 
    background->AddComponent(std::make_shared<Sprite>(*background_spr));    
    
    background->box.x = (RESOLUTION_WIDTH )/2 - background->box.w/2;   
    background->box.y = 0; 

    CameraFollower* background_cmfl = new CameraFollower(*background);
    background->AddComponent(std::make_shared<CameraFollower>(*background_cmfl));

    
    Game::GetInstance().GetCurrentState().AddObject(background); 
}

void SkillSelection::CreatePassButon() {
    passButon = new GameObject(0, 0);
    Sprite* passButon_spr = new Sprite(*passButon, SKILL_SELECTION_ARROW); 
    passButon->AddComponent(std::make_shared<Sprite>(*passButon_spr));    
    
    passButon->box.x =  background->box.x + background->box.w - passButon->box.w - OFFSET_SKILL_SELECTION_ARROW.x;   
    passButon->box.y = background->box.y + background->box.h - passButon->box.h - OFFSET_SKILL_SELECTION_ARROW.y; 

    CameraFollower* passButon_cmfl = new CameraFollower(*passButon);
    passButon->AddComponent(std::make_shared<CameraFollower>(*passButon_cmfl));


    Game::GetInstance().GetCurrentState().AddObject(passButon); 
}

void SkillSelection::CreateSkillOptions() {
    // Get all skill IDs from Skill::skillInfoMap up to LOCKED1 (exclusive)
    auto itEnd = Skill::skillInfoMap.find(Skill::InstantRegret);
    std::vector<Skill::SkillId> validSkillIds;
    for (auto it = Skill::skillInfoMap.begin(); it != itEnd; ++it) {
        validSkillIds.push_back(it->first);
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
        Skill *skillSprite = new Skill(*skillObject, skillId, nullptr, false);
        skillObject->AddComponent(std::shared_ptr<Skill>(skillSprite));

        // Set the position and add to the current state
        skillObject->box.x = background->box.x + background->box.w / 2 - skillObject->box.w / 2 - OFFSET_SKILL_OPTIONS + OFFSET_SKILL_OPTIONS * i;
        skillObject->box.y = background->box.y + OFFSET_SKILL_OPTIONSY;

        CameraFollower *skill_cmfl = new CameraFollower(*skillObject);
        skillObject->AddComponent((std::shared_ptr<CameraFollower>)skill_cmfl);

        Game::GetInstance().GetCurrentState().AddObject(skillObject);
    }
}
 
void SkillSelection::Update(float dt) {

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
                passButon->RemoveComponent(arrowComponentPtr);
                Sprite *passButon_spr = new Sprite(*passButon, SKILL_SELECTION_ARROW_ACTIVATED); 
                passButon->AddComponent(std::shared_ptr<Sprite>(passButon_spr)); 
            } 
        }       
    }

    else if((Skill::skillFromReward == nullptr || Skill::skillToReward == nullptr) && toggleArrow){
        toggleArrow = false;

        if(passButon != nullptr){
            auto arrowComponent = passButon->GetComponent("Sprite"); 
            auto arrowComponentPtr = std::dynamic_pointer_cast<Sprite>(arrowComponent);
            if (arrowComponentPtr) {
                passButon->RemoveComponent(arrowComponentPtr);
                Sprite *passButon_spr = new Sprite(*passButon, SKILL_SELECTION_ARROW); 
                passButon->AddComponent(std::shared_ptr<Sprite>(passButon_spr)); 
            }
        }    
    }

    Vec2 mousePos(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());
    


    if (passButon->box.Contains(mousePos.x- Camera::pos.x, mousePos.y- Camera::pos.y)) {
        if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
            if(Skill::skillFromReward != nullptr && Skill::skillToReward != nullptr){
                auto skillFrom = Skill::skillFromReward->GetId();
                auto skillTo = Skill::skillToReward->GetId();
                Skill::AddSkill(skillFrom, skillTo);
                Skill::skillFromReward = nullptr;
                Skill::skillToReward = nullptr;
            }
        }

        if(passButon != nullptr){
            auto arrowComponent = passButon->GetComponent("Sprite"); 
            auto arrowComponentPtr = std::dynamic_pointer_cast<Sprite>(arrowComponent);
                // Center position original
                auto posXenterX = passButon->box.x + passButon->box.w / 2;
                auto posXenterY = passButon->box.y + passButon->box.h / 2;

                // Call SetScale with the correct number of arguments
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
                std::cout << passButon->box.x << std::endl;
                passButon->box.x = posXenterX - passButon->box.w / 2;
                passButon->box.y = posXenterY - passButon->box.h / 2;
                
                std::cout << passButon->box.x << " " << passButon->box.w / 2<<" " <<posXenterX << std::endl;
        }
    }  
    

}

void SkillSelection::Render() {
}
 
bool SkillSelection::Is(std::string type) { 
    return (type == "SkillSelection");
}
