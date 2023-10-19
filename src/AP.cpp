#include "AP.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprite.h"

// speed já está sendo inicializado pelo construtor de Vec2
AP::AP(GameObject &associated)
: Component::Component(associated),
apCount(3)

{   
    for (int i = 0; i < 3; ++i) {
        GameObject* apObj = new GameObject(associated.box.x, associated.box.y);
        apObj->box.y += i * AP_VERTICAL_SPACING;
        Game::GetInstance().GetCurrentState().AddObject(apObj);

        // Configure os sprites com base no valor de apCount
        std::string spritePath = (apCount > i) ? AP_FULL_SPRITE : AP_EMPTY_SPRITE; 
        Sprite* ap_spr = new Sprite(*apObj, spritePath); 
        associated.AddComponent(std::shared_ptr<Sprite>(ap_spr)); 
    }
        

} 
   
void AP::Start() {     
    
}  
 
AP::~AP(){ 
    
} 

void AP::Update(float dt){  

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
    // Atualiza a representação visual com base no novo valor de apCount
    //for (int i = 0; i < 3; ++i) {
        //GameObject *apObj = Game::GetInstance().GetCurrentState().GetObjectPtr(i);
        //Sprite *oldSprite = apObj->GetComponent("Sprite");

        //if (oldSprite) {
        //    apObj->RemoveComponent(oldSprite);
        //}

        // Configure os sprites com base no valor de apCount
        //std::string spritePath = (apCount > i) ? AP_FULL_SPRITE : AP_EMPTY_SPRITE;
        //Sprite *ap_spr = new Sprite(*apObj, spritePath);
        //apObj->AddComponent(std::shared_ptr<Sprite>(ap_spr));

    //}    
}

void AP::MirrorAPCount(int mirrorAPCount) {
    // Cria uma representação visual de espelho com base em mirrorAPCount
    GameObject mirrorObject;
    mirrorObject.box.x = associated.box.x + 100;  // Posição horizontal do espelho
    mirrorObject.box.y = associated.box.y;
    InitVisualRepresentationForMirror(mirrorAPCount, mirrorObject);
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

