#include "AP.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "GameData.h"
#include "Enemies.h"
#include "Mother.h"
#include "Daughter.h"
#include "Sprite.h"
#include "CameraFollower.h"
#include "SkillSelection.h"
#include "CombatState.h"
#include "UI.h"

#include <algorithm> 

std::vector<std::weak_ptr<GameObject>> AP::apArray;

int AP::apCount = AP_QUANTITY;

AP::AP(GameObject &associated)
: Component::Component(associated)

{   
    for (int i = 0; i < AP_QUANTITY; ++i) {
        GameObject *apObj = new GameObject(associated.box.x, associated.box.y - 20); // Use shared_ptr
        
        if(i == 0){
            apObj->box.x += AP_POS1.x ;
            apObj->box.y += AP_POS1.y;
        }
        else if(i == 1){ 
            apObj->box.x += AP_POS2.x;
            apObj->box.y += AP_POS2.y;
        } 
        else if(i == 2){ 
            apObj->box.x += AP_POS3.x;
            apObj->box.y += AP_POS3.y;
        }

        // Configure the sprites based on the value of apCount
        std::string spritePath = (apCount > i) ? AP_FULL_SPRITE : AP_EMPTY_SPRITE;
        Sprite* ap_spr = new Sprite(*apObj, spritePath); // Use shared_ptr
        apObj->AddComponent((std::shared_ptr<Component>)ap_spr);

        CameraFollower* ap_UI_cmfl = new CameraFollower(*apObj);
        apObj->AddComponent((std::shared_ptr<CameraFollower>)ap_UI_cmfl);

        auto weak_ap = Game::GetInstance().GetCurrentState().AddObject(apObj);

        apArray.push_back(weak_ap);
        
    } 
        

} 
   
void AP::Start() {     
    AP::apCount = AP_QUANTITY;
}  
 
AP::~AP(){ 
    std::cout << "cgegou aq22io aps" << std::endl;
    for (int i = AP::apArray.size() - 1; i >= 0; i--) { //remove enemies tags
            AP::apArray.erase(AP::apArray.begin() + i);
    }
    std::cout << "cgegou aq22io ape" << std::endl;
} 

void AP::Update(float dt){  


    if(!SkillSelection::skillSelectionActivated){


        //Change to enemy turn
        if((apCount == 0 && GameData::playerTurn == true && Skill::selectedSkill == nullptr &&
        Skill::skillBackToDaughter == nullptr && Skill::skillBackToMother == nullptr) || (UI::nextActivated && !SkillSelection::skillSelectionActivated)){
            
            delayChangeSides.Update(dt);
            if((delayChangeSides.Get() > DELAY_CHANGE_SIDES || (UI::nextActivated && !SkillSelection::skillSelectionActivated)) && CombatState::ChangingSides == false){
                GameData::playerTurn = false;
                Skill::selectedSkill = nullptr;
                if(Enemies::enemiesToAttack <= 0){//init enemies attack turn
                    Enemies::enemiesToAttack = Enemies::enemiesCount;
                } 

                //Camera focus
                GameObject* focusCamera =  new GameObject(FOCUS_ENEMY, 0);
                Camera::Follow(focusCamera);
                CombatState::ChangingSides = true;
                delayChangeSides.Restart();
                UI::nextActivated = false;

            }

            
        }
 
        //Change to player turn
        if(Enemies::enemiesToAttack == 0 && GameData::playerTurn == false && !Enemies::enemyAttacking){ 
            
            delayChangeSides.Update(dt);
            if(delayChangeSides.Get() > DELAY_CHANGE_SIDES && CombatState::ChangingSides == false){
                GameData::playerTurn = true;
                AP::apCount = AP_QUANTITY; //reset
                SetAPCount(AP::apCount);

                //Camera focus
                GameObject* focusCamera =  new GameObject(-FOCUS_ENEMY, 0);
                Camera::Follow(focusCamera);
                CombatState::ChangingSides = true;
                
                if(Enemies::enemiesCount > 0){
                    // Remove one count of all tag drom which enemy
                    for (const std::weak_ptr<GameObject>& enemy : Enemies::enemiesArray) {
                        auto enemyPtr = enemy.lock();
                        if (enemyPtr) {
                            auto enemiesComponent = enemyPtr->GetComponent("Enemies");
                            if (enemiesComponent) {
                                auto enemies = std::dynamic_pointer_cast<Enemies>(enemiesComponent);
                                enemies->RemoveOneTagAll();
                            }
                        }
                    }

                    auto motherComponent = Mother::motherInstance.lock()->GetComponent("Mother");
                    if (motherComponent) {
                        auto mother = std::dynamic_pointer_cast<Mother>(motherComponent);
                        mother->RemoveOneTagAll();
                    }

                    auto daughterComponent = Daughter::daughterInstance.lock()->GetComponent("Daughter");
                    if (daughterComponent) {
                        auto daughter = std::dynamic_pointer_cast<Daughter>(daughterComponent);
                        daughter->RemoveOneTagAll();
                    }
                }
                
                delayChangeSides.Restart(); 
            }
        } 
    }
    else{
        if(AP::apCount != AP_QUANTITY){
            AP::apCount = AP_QUANTITY; //reset
            SetAPCount(AP::apCount);
        }
        
    }
    

}

void AP::Render(){
} 

void AP::SetAPCount(int newAPCount) { 
    if (newAPCount >= 0 && newAPCount <= 3) {
        apCount = newAPCount;
    }
    UpdateVisualRepresentation();
}

void AP::UpdateVisualRepresentation() {
    int index = AP_QUANTITY;
    for (auto& apGameObject :apArray) {
        if (apGameObject.lock()) {
        // Try to retrieve the "Sprite" component
            auto spriteComponent = apGameObject.lock()->GetComponent("Sprite");
            auto spriteComponentPtr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
            if (spriteComponentPtr) {
                apGameObject.lock()->RemoveComponent(spriteComponentPtr);
            }

            std::string spritePath = (apCount < index) ? AP_EMPTY_SPRITE : AP_FULL_SPRITE;
            Sprite* ap_spr = new Sprite(*apGameObject.lock(), spritePath); // Use shared_ptr
            apGameObject.lock()->AddComponent((std::shared_ptr<Component>)ap_spr);
            index--;
        }
    }            
}

void AP::MirrorAPCount(int mirrorAPCount) {
    int fullCount = mirrorAPCount; // Calculate the number of full sprites
    int index = AP_QUANTITY; 
    
    for (auto& apGameObject : apArray) {
        if (apGameObject.lock()) {
            // Try to retrieve the "Sprite" component
            auto spriteComponent = apGameObject.lock()->GetComponent("Sprite");
            auto spriteComponentPtr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
            if (spriteComponentPtr) {
                apGameObject.lock()->RemoveComponent(spriteComponentPtr);
            }

            std::string spritePath = (fullCount < index) ? AP_MIRROR_SPRITE : AP_FULL_SPRITE;
            spritePath = (apCount < index) ? AP_EMPTY_SPRITE : spritePath;
            
            Sprite* ap_spr = new Sprite(*apGameObject.lock(), spritePath); // Use shared_ptr
            apGameObject.lock()->AddComponent((std::shared_ptr<Component>)ap_spr);
            index--;
        }
    }       
}


bool AP::Is(std::string type){
    return (type == "AP");
}

