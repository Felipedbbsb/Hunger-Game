#include "UI.h"
#include "Game.h"
#include "Camera.h"

// speed já está sendo inicializado pelo construtor de Vec2
// UI.h
UI::UI(GameObject &associated, std::vector<std::shared_ptr<Skill>> SkillNormalArray)
    : Component(associated), SkillNormalArray(SkillNormalArray) {
    // Add o sprite
    Sprite *ui_screen_spr = new Sprite(associated, UI_SCREEN_SPRITE);
    associated.AddComponent((std::shared_ptr<Sprite>)ui_screen_spr);
} 
 


UI::~UI()
{
     
}



void UI::AddSkill(std::shared_ptr<Skill> skill) {
    SkillNormalArray.push_back(skill);
}

void UI::Start() { 


    for (int i = SkillNormalArray.size() - 1; i >= 0; i--) {
        int offsetArray = SkillNormalArray.size() - i;
        GameObject* normalSkill = new GameObject(150 * offsetArray + 100, RESOLUTION_HEIGHT * 2/3 + 75);
        // Acesse o Skill::SkillId a partir do std::shared_ptr<Skill>
        Skill::SkillId skillId = SkillNormalArray[i]->GetId();

        Skill* skill_behaviour = new Skill(*normalSkill, skillId);
        normalSkill->AddComponent(std::shared_ptr<Skill>(skill_behaviour));
        Game::GetInstance().GetCurrentState().AddObject(normalSkill);
        
    }

}

void UI::Update(float dt) {

}

void UI::Render() {

}




bool UI::Is(std::string type){
    return (type == "UI");
}

