#include "AP.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "GameData.h"
#include "Enemies.h"
#include "Sprite.h"
#include <algorithm> 

std::vector<std::shared_ptr<GameObject>> AP::apArray;

int AP::apCount = AP_QUANTITY;

AP::AP(GameObject &associated)
: Component::Component(associated)

{   
    for (int i = 0; i < AP_QUANTITY; ++i) {
        std::shared_ptr<GameObject> apObj = std::make_shared<GameObject>(associated.box.x, associated.box.y); // Use shared_ptr
        apObj->box.y += i * AP_VERTICAL_SPACING;

        // Configure the sprites based on the value of apCount
        std::string spritePath = (apCount > i) ? AP_FULL_SPRITE : AP_EMPTY_SPRITE;
        std::shared_ptr<Sprite> ap_spr = std::make_shared<Sprite>(*apObj, spritePath); // Use shared_ptr
        apObj->AddComponent(ap_spr);
        Game::GetInstance().GetCurrentState().AddObject(apObj.get());

        apArray.push_back(apObj);
        
    } 
        

} 
   
void AP::Start() {     
    
}  
 
AP::~AP(){ 
    
} 

void AP::Update(float dt){  
    if(apCount == 0 && GameData::playerTurn == true){
        GameData::playerTurn = false;
        std::cout << "chico mkjilo" << std::endl;
        if(Enemies::enemiesToAttack == 0){//init enemies attack turn
            Enemies::enemiesToAttack = Enemies::enemiesCount;
        } 
    }

    if(Enemies::enemiesToAttack == 0 && !Enemies::enemyAttacking && GameData::playerTurn == false){ 
        GameData::playerTurn = true;
        AP::apCount = AP_QUANTITY;
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

