#include "Tag.h"
#include "Text.h"
#include "Game.h"
#include "Reader.h"

Tag::Tag(GameObject &associated, Skill::SkillsTags tag, std::weak_ptr<GameObject> enemyRef, int quantity)
: Component::Component(associated),
enemyRef(enemyRef),
quantity(quantity),
tag(tag),
readerTag(nullptr)
{   if (enemyRef.lock()){
        std::string spriteTag;
        switch (tag) {
            case Skill::SkillsTags::RESILIENCE: spriteTag = TAG_RESILIENCE_SPRITE; break;
            case Skill::SkillsTags::DODGE: spriteTag = TAG_DODGE_SPRITE; break;
            case Skill::SkillsTags::PROVOKE: spriteTag = TAG_PROVOKE_SPRITE; break;
            case Skill::SkillsTags::VULNERABLE: spriteTag = TAG_VULNERABLE_SPRITE; break;
            case Skill::SkillsTags::WEAK: spriteTag = TAG_WEAK_SPRITE; break;
            case Skill::SkillsTags::RAMPAGE: spriteTag = TAG_RAMPAGE_SPRITE; break;
            case Skill::SkillsTags::PROTECTED: spriteTag = TAG_PROTECTED_SPRITE; break;
        } 
        
        Sprite* tag_spr = new Sprite(associated, spriteTag);
        tag_spr->SetScale(0.07, 0.07);
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

Skill::SkillsTags Tag::GetTag(){
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
        case Skill::SkillsTags::RESILIENCE:
            tagName = "Resilience";
            break;
        case Skill::SkillsTags::DODGE:
            tagName = "Dodge";
            break;
        case Skill::SkillsTags::PROVOKE:
            tagName = "Provoke";
            break;
        case Skill::SkillsTags::VULNERABLE:
            tagName = "Vulnerable";
            break;
        case Skill::SkillsTags::WEAK:
            tagName = "Weak";
            break;
        case Skill::SkillsTags::RAMPAGE:
            tagName = "Rampage";
            break;
        case Skill::SkillsTags::PROTECTED: 
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

