#include "Tag.h"
#include "Text.h"
#include "Game.h"


Tag::Tag(GameObject &associated, Skill::SkillsTags tag )
: Component::Component(associated)

{ 
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
    
    auto tag_spr = std::make_shared<Sprite>(associated, spriteTag);
    tag_spr->SetScale(0.1, 0.1);
    associated.AddComponent(tag_spr);

} 
 
void Tag::Start() {     
   
}  
 
Tag::~Tag(){ 
    
} 

void Tag::Update(float dt){  

}

void Tag::Render(){
} 

bool Tag::Is(std::string type){
    return (type == "Tag");
}

