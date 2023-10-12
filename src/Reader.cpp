#include "Reader.h"
#include "Text.h"
#include "Game.h"

// speed já está sendo inicializado pelo construtor de Vec2
Reader::Reader(GameObject &associated, std::weak_ptr<GameObject> skillRef, std::string textSkill )
: Component::Component(associated),
skillRef(skillRef),
textSkillObj(nullptr),
textSkill(textSkill)
{ 
     // Adicione um sprite
    Sprite *reader_spr = new Sprite(associated, READER_SPRITE);
    reader_spr->SetScale(0.25, 0.25);
    associated.AddComponent(std::shared_ptr<Sprite>(reader_spr));
  
        
} 
 
void Reader::Start() {     
    // Create textSkillObj 
    std::shared_ptr<GameObject> skillRef_obj = skillRef.lock();
 
    textSkillObj = new GameObject();
    Text *textSkillObjString = new Text(*textSkillObj, TEXT_SKILL_FONT, TEXT_SKILL_SIZE, Text::BLENDED, textSkill, TEXT_SKILL_FONT_COLOR, 0);;
    textSkillObj->AddComponent(std::shared_ptr<Component>(textSkillObjString)); 
    textSkillObj->box.x = skillRef_obj->box.x + 10;
    textSkillObj->box.y = skillRef_obj->box.y + 10;
    //textSkillObj->box.w = skillRef_obj->box.w;  
    //textSkillObj->box.h = skillRef_obj->box.h;  
    Game::GetInstance().GetCurrentState().AddObject(textSkillObj); 
}  
 
Reader::~Reader(){ 
    if (textSkillObj) {
        textSkillObj->RequestDelete(); // Delete  textSkillObj with exists
    }
    
} 

void Reader::Update(float dt){  

}

void Reader::Render(){
} 

bool Reader::Is(std::string type){
    return (type == "Reader");
}

