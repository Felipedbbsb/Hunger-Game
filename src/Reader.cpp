#include "Reader.h"
#include "Text.h"
#include "Game.h"

// speed já está sendo inicializado pelo construtor de Vec2
Reader::Reader(GameObject &associated, std::weak_ptr<GameObject> skillRef) : Component::Component(associated),
skillRef(skillRef),
textSkill(nullptr)
{
     // Adicione um sprite
    Sprite *reader_spr = new Sprite(associated, READER_SPRITE);
    reader_spr->SetScale(0.25, 0.25);
    associated.AddComponent(std::shared_ptr<Sprite>(reader_spr));
 
      
} 
 
void Reader::Start() {     
    // Create textSkill 
    std::shared_ptr<GameObject> skillRef_obj = skillRef.lock();
 
    textSkill = new GameObject();
    Text *textSkillString = new Text(*textSkill, TEXT_SKILL_FONT, TEXT_SKILL_SIZE, Text::BLENDED, TEXT_SKILL, TEXT_SKILL_FONT_COLOR, 0);;
    textSkill->AddComponent(std::shared_ptr<Component>(textSkillString)); 
    textSkill->box.x = skillRef_obj->box.x + 10;
    textSkill->box.y = skillRef_obj->box.y + 10;
    //textSkill->box.w = skillRef_obj->box.w;  
    //textSkill->box.h = skillRef_obj->box.h;  
    Game::GetInstance().GetCurrentState().AddObject(textSkill); 
}  
 
Reader::~Reader(){ 
    if (textSkill) {
        textSkill->RequestDelete(); // Delete  textSkill with exists
    }
    
} 

void Reader::Update(float dt){  

}

void Reader::Render(){
}

bool Reader::Is(std::string type){
    return (type == "Reader");
}

