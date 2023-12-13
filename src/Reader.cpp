#include "Reader.h"
#include "Text.h"
#include "Game.h"
#include "Camera.h"
#include "CameraFollower.h"
// speed já está sendo inicializado pelo construtor de Vec2
Reader::Reader(GameObject &associated, std::string textSkill, std::string readerPath )
: Component::Component(associated),
textSkillObj(nullptr),
textSkill(textSkill),
readerPath(readerPath)
{   
     // Adicione um sprite
    if(textSkill.find("assets/img") != 0){ 
        Sprite *reader_spr = new Sprite(associated, READER_SPRITE);
        associated.AddComponent(std::shared_ptr<Sprite>(reader_spr));    
        CameraFollower *textSkillObj_cmfl = new CameraFollower(associated);
        associated.AddComponent((std::shared_ptr<CameraFollower>)textSkillObj_cmfl);
    }
    else{
        Sprite *reader_spr = new Sprite(associated, textSkill);
        associated.AddComponent(std::shared_ptr<Sprite>(reader_spr));
    }    
}  
  
void Reader::Start() {     

    // Create textSkillObj  
    if(textSkill.find("assets/img") != 0){
        textSkillObj = new GameObject();
        Text *textSkillObjString = new Text(*textSkillObj, TEXT_SKILL_FONT, TEXT_SKILL_SIZE, Text::BLENDED, textSkill, TEXT_SKILL_FONT_COLOR, 0);;
        textSkillObj->AddComponent(std::shared_ptr<Component>(textSkillObjString)); 
        textSkillObj->box.x = associated.box.x + 10;
        textSkillObj->box.y = associated.box.y + 10;


        Game::GetInstance().GetCurrentState().AddObject(textSkillObj); 
    }
    
}  
 
Reader::~Reader(){ 
        std::cout << "aaaaaaaa reader start" << std::endl;

    if (textSkillObj) {
        textSkillObj->RequestDelete(); // Delete  textSkillObj with exists
    }
        std::cout << "aaaaaaaa reader end" << std::endl;

} 

void Reader::Update(float dt){  
    if(textSkillObj != nullptr){
        textSkillObj->box.x = associated.box.x + 10;
        textSkillObj->box.y = associated.box.y + 10;
    }
}

void Reader::Render(){
} 

bool Reader::Is(std::string type){
    return (type == "Reader");
}

