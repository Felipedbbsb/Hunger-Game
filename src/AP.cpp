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

#include <algorithm> 

std::vector<std::shared_ptr<GameObject>> AP::apArray;

int AP::apCount = AP_QUANTITY;

AP::AP(GameObject &associated)
: Component::Component(associated)

{   
    for (int i = 0; i < AP_QUANTITY; ++i) {
        std::shared_ptr<GameObject> apObj = std::make_shared<GameObject>(associated.box.x, associated.box.y - 20); // Use shared_ptr
        
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
        std::shared_ptr<Sprite> ap_spr = std::make_shared<Sprite>(*apObj, spritePath); // Use shared_ptr
        apObj->AddComponent(ap_spr);

        std::shared_ptr<CameraFollower> ap_UI_cmfl = std::make_shared<CameraFollower>(*apObj);
        apObj->AddComponent(ap_UI_cmfl);

        Game::GetInstance().GetCurrentState().AddObject(apObj.get());

        apArray.push_back(apObj);
        
    } 
        

} 
   
void AP::Start() {     
    
}  
 
AP::~AP(){ 
    
} 

void AP::Update(float dt){  
    if(!SkillSelection::skillSelectionActivated){
        if(apCount == 0 && GameData::playerTurn == true && Skill::selectedSkill == nullptr &&
        Skill::skillBackToDaughter == nullptr && Skill::skillBackToMother == nullptr){
            GameData::playerTurn = false;
            if(Enemies::enemiesToAttack <= 0){//init enemies attack turn
                Enemies::enemiesToAttack = Enemies::enemiesCount;
            } 
        }

        if(Enemies::enemiesToAttack == 0 && GameData::playerTurn == false && !Enemies::enemyAttacking){ 
            GameData::playerTurn = true;
            AP::apCount = AP_QUANTITY; //reset
            SetAPCount(AP::apCount);

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
    }
    else{
        AP::apCount = AP_QUANTITY; //reset
        SetAPCount(AP::apCount);
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
        if (apGameObject) {
        // Try to retrieve the "Sprite" component
            auto spriteComponent = apGameObject->GetComponent("Sprite");
            auto spriteComponentPtr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
            if (spriteComponentPtr) {
                apGameObject->RemoveComponent(spriteComponentPtr);
            }

            std::string spritePath = (apCount < index) ? AP_EMPTY_SPRITE : AP_FULL_SPRITE;
            std::shared_ptr<Sprite> ap_spr = std::make_shared<Sprite>(*apGameObject, spritePath); // Use shared_ptr
            apGameObject->AddComponent(ap_spr);
            index--;
        }
    }            
}

void AP::MirrorAPCount(int mirrorAPCount) {
    int fullCount = mirrorAPCount; // Calculate the number of full sprites
    int index = AP_QUANTITY; 
    
    for (auto& apGameObject : apArray) {
        if (apGameObject) {
            // Try to retrieve the "Sprite" component
            auto spriteComponent = apGameObject->GetComponent("Sprite");
            auto spriteComponentPtr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
            if (spriteComponentPtr) {
                apGameObject->RemoveComponent(spriteComponentPtr);
            }

            std::string spritePath = (fullCount < index) ? AP_MIRROR_SPRITE : AP_FULL_SPRITE;
            spritePath = (apCount < index) ? AP_EMPTY_SPRITE : spritePath;
            
            std::shared_ptr<Sprite> ap_spr = std::make_shared<Sprite>(*apGameObject, spritePath);
            apGameObject->AddComponent(ap_spr);
            index--;
        }
    }       
}


bool AP::Is(std::string type){
    return (type == "AP");
}

