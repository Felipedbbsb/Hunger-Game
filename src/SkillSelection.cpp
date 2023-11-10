#include "SkillSelection.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Reader.h"
#include "GameData.h"
#include "CombatState.h"
#include "InteractionObject.h"
#include "CameraFollower.h"

#include <algorithm>

bool SkillSelection::skillSelectionActivated = false;

SkillSelection::SkillSelection(GameObject &associated)
    : Component(associated),
    background(nullptr)
    {
        SkillSelection::skillSelectionActivated = true;

        Skill::selectedSkill = nullptr; //generic 

        Skill::selectedSkillEnemy = nullptr; //generic 

        Skill::skillBackToMother = nullptr; //back effects

        Skill::skillBackToDaughter = nullptr; //back effects 
}
  
void SkillSelection::Start() {
    background = new GameObject();
    Sprite *background_spr = new Sprite(*background, SKILL_SELECTION_BCKGROUND); 
    background->AddComponent(std::shared_ptr<Sprite>(background_spr));    
    
    background->box.x = (RESOLUTION_WIDTH )/2 - background->box.w/2;   
    background->box.y = 0; 

    CameraFollower *background_cmfl = new CameraFollower(*background);
    background->AddComponent((std::shared_ptr<CameraFollower>)background_cmfl);

    
    Game::GetInstance().GetCurrentState().AddObject(background); 
}   



SkillSelection::~SkillSelection() {
    
}
 
void SkillSelection::Update(float dt) {

}

void SkillSelection::Render() {
}
 
bool SkillSelection::Is(std::string type) { 
    return (type == "SkillSelection");
}
