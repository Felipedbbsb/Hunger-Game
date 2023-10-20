#include "AP.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprite.h"
#include <algorithm> 

std::vector<std::shared_ptr<GameObject>> AP::apArray;


AP::AP(GameObject &associated)
: Component::Component(associated),
apCount(2)

{   
    for (int i = 0; i < 3; ++i) {
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
    MirrorAPCount(1);
    //SetAPCount(3);
}

void AP::Render(){
} 

void AP::SetAPCount(int newAPCount) {
    if (newAPCount >= 0 && newAPCount <= 3) {
        apCount = newAPCount;
        UpdateVisualRepresentation();
    }
}

void AP::UpdateVisualRepresentation() {
    int index = 3;
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
    int index = 3;
    
    for (auto& apGameObject : apArray) {
        if (apGameObject) {
            // Try to retrieve the "Sprite" component
            auto spriteComponent = apGameObject->GetComponent("Sprite");
            auto spriteComponentPtr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
            if (spriteComponentPtr) {
                apGameObject->RemoveComponent(spriteComponentPtr);
            }

            std::string spritePath = (apCount < index) ? AP_EMPTY_SPRITE : AP_FULL_SPRITE;
            spritePath = (fullCount < index) ? AP_MIRROR_SPRITE : spritePath;
            std::shared_ptr<Sprite> ap_spr = std::make_shared<Sprite>(*apGameObject, spritePath);
            apGameObject->AddComponent(ap_spr);
            index--;
        }
    }       
}

void AP::InitVisualRepresentationForMirror(int mirrorAPCount, GameObject &mirrorObject) {
    for (int i = 0; i < 3; ++i) {
        GameObject* apObj = new GameObject(mirrorObject.box.x, mirrorObject.box.y);
        apObj->box.y += i * AP_VERTICAL_SPACING;
        Game::GetInstance().GetCurrentState().AddObject(apObj);

        // Configura os sprites com base no valor de mirrorAPCount
        std::string spritePath = (mirrorAPCount > i) ? AP_FULL_SPRITE : AP_EMPTY_SPRITE;
        Sprite* ap_spr = new Sprite(*apObj, spritePath);
    }
}

bool AP::Is(std::string type){
    return (type == "AP");
}

