#include "UI.h"
#include "Game.h"
#include "Camera.h"
#include "AP.h"

// speed já está sendo inicializado pelo construtor de Vec2
// UI.h
UI::UI(GameObject &associated, std::vector<Skill::SkillId> SkillNormalArray, std::vector<Skill::SkillId> SkillDjinArray)
    : Component(associated), SkillNormalArray(SkillNormalArray), SkillDjinArray(SkillDjinArray){
    // Add o sprite
    Sprite *ui_screen_spr = new Sprite(associated, UI_SCREEN_SPRITE);
    associated.AddComponent((std::shared_ptr<Sprite>)ui_screen_spr);
} 
 


UI::~UI() 
{
     
}



void UI::AddSkill(Skill::SkillId skill) {
    SkillNormalArray.push_back(skill);
}

void UI::Start() { 

    GameObject* ap_UI = new GameObject(AP_POS);
        AP* ap_behaviour = new AP(*ap_UI);
        ap_UI->AddComponent(std::shared_ptr<AP>(ap_behaviour));
        Game::GetInstance().GetCurrentState().AddObject(ap_UI);



    int offsetArray = 0;
    int ofsArrayIn = 0; // offset distance between normal and Djinn array

    for (int i = SkillNormalArray.size() - 1; i >= 0; i--) {
        int offsetArray = SkillNormalArray.size() - i - 1;
        GameObject* normalSkill = new GameObject(150 * offsetArray + 250, RESOLUTION_HEIGHT * 2/3 + 75);
        // Acesse o Skill::SkillId a partir do std::shared_ptr<Skill>
        Skill::SkillId skillId = SkillNormalArray[i];

        Skill* skill_behaviour = new Skill(*normalSkill, skillId, ap_behaviour);
        normalSkill->AddComponent(std::shared_ptr<Skill>(skill_behaviour));
        Game::GetInstance().GetCurrentState().AddObject(normalSkill);
        if(i >= 0){
            ofsArrayIn = 150 * offsetArray + 300; 
        }
    }

     for (int i = SkillDjinArray.size() - 1; i >= 0; i--) {
        offsetArray = SkillDjinArray.size() - i - 1;
        GameObject* normalSkill = new GameObject(ofsArrayIn + 150 * offsetArray + 150, RESOLUTION_HEIGHT * 2/3 + 75);
        // Acesse o Skill::SkillId a partir do std::shared_ptr<Skill>
        Skill::SkillId skillId = SkillDjinArray[i];

        Skill* skill_behaviour = new Skill(*normalSkill, skillId, ap_behaviour);
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

