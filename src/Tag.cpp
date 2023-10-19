#include "Tag.h"
#include "Text.h"
#include "Game.h"
#include "Reader.h"

Tag::Tag(GameObject &associated, Tags tag, std::weak_ptr<GameObject> enemyRef, int quantity)
: Component::Component(associated),
enemyRef(enemyRef),
quantity(quantity), 
tag(tag),
readerTag(nullptr),
bigTag(nullptr) 
{   if (enemyRef.lock()){
        std::string spriteTag;
        switch (tag) {
            case Tag::Tags::RESILIENCE: spriteTag = TAG_RESILIENCE_SPRITE; break;
            case Tag::Tags::DODGE: spriteTag = TAG_DODGE_SPRITE; break;
            case Tag::Tags::PROVOKE: spriteTag = TAG_PROVOKE_SPRITE; break;
            case Tag::Tags::VULNERABLE: spriteTag = TAG_VULNERABLE_SPRITE; break;
            case Tag::Tags::WEAK: spriteTag = TAG_WEAK_SPRITE; break;
            case Tag::Tags::RAMPAGE: spriteTag = TAG_RAMPAGE_SPRITE; break;
            case Tag::Tags::PROTECTED: spriteTag = TAG_PROTECTED_SPRITE; break;
        } 
        
        Sprite* tag_spr = new Sprite(associated, spriteTag);
        associated.AddComponent(std::shared_ptr<Sprite>(tag_spr));


    }
    else{
        associated.RequestDelete();
    }
} 
 
void Tag::Start() {     
   
}  
  
Tag::~Tag(){ 
    HideReader();
} 



void Tag::Update(float dt){  
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());

    if(auto ref = enemyRef.lock()){
        if(associated.box.Contains(mousePos.x, mousePos.y)){
            ShowReader();
        } else {
            HideReader();
        }
        
    }
    else{
        associated.RequestDelete();
    }

    

} 

void Tag::UpdateQuantity(int newQuantity) {
    quantity = newQuantity;
}

Tag::Tags Tag::GetTag(){
    return tag;
}

void Tag::ShowReader(){
    if (!readerTag) {
        readerTag = new GameObject(associated.box.x, associated.box.y);
        Reader* readerTag_behaviour = new Reader(*readerTag, GetTagMessage());
        readerTag->AddComponent(std::shared_ptr<Reader>(readerTag_behaviour));
        Game::GetInstance().GetCurrentState().AddObject(readerTag);
    }
}

void Tag::HideReader() {
    if (readerTag!= nullptr) {
        readerTag->RequestDelete();
        readerTag = nullptr;
    } 
}


std::string Tag::GetTagMessage() {
    std::string tagName = GetTagName();
    std::string message = tagName + " (" + std::to_string(quantity) + " Turns)";
    return message; 
}


std::string Tag::GetTagName() {
    std::string tagName;

    switch (tag) {
        case Tags::RESILIENCE:
            tagName = "Resilience";
            break;
        case Tags::DODGE:
            tagName = "Dodge";
            break;
        case Tags::PROVOKE:
            tagName = "Provoke";
            break;
        case Tags::VULNERABLE:
            tagName = "Vulnerable";
            break;
        case Tags::WEAK:
            tagName = "Weak";
            break;
        case Tags::RAMPAGE:
            tagName = "Rampage";
            break;
        case Tags::PROTECTED: 
            tagName = "Protected";
            break;
        default:
            tagName = "Unknown";
    }

    return tagName;
}

void Tag::Render(){
} 

bool Tag::Is(std::string type){
    return (type == "Tag");
}

