#include "UI.h"
#include "Game.h"
#include "Camera.h"
#include "AP.h"
#include "Protected.h"
#include "CameraFollower.h"

// speed já está sendo inicializado pelo construtor de Vec2
// UI.h
UI::UI(GameObject &associated, std::vector<Skill::SkillId> SkillNormalArray, std::vector<Skill::SkillId> SkillDjinArray)
    : Component(associated), SkillNormalArray(SkillNormalArray), SkillDjinArray(SkillDjinArray){
    
    //HUD backgorund 
    GameObject *ui_BG = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    Sprite *ui_screen_BG_spr = new Sprite(*ui_BG, UI_SCREEN_BG_SPRITE);
    CameraFollower *ui_BG_cmfl = new CameraFollower(*ui_BG);
    ui_BG->AddComponent((std::shared_ptr<CameraFollower>)ui_BG_cmfl);
    ui_BG->AddComponent((std::shared_ptr<Sprite>)ui_screen_BG_spr);
    Game::GetInstance().GetCurrentState().AddObject(ui_BG);   

    //AP
    GameObject* ap_UI = new GameObject(associated.box.x , associated.box.y);
        AP* ap_behaviour = new AP(*ap_UI);
        CameraFollower *ap_UI_cmfl = new CameraFollower(*ap_UI);
        ap_UI->AddComponent((std::shared_ptr<CameraFollower>)ap_UI_cmfl);
        ap_UI->AddComponent(std::shared_ptr<AP>(ap_behaviour));
        Game::GetInstance().GetCurrentState().AddObject(ap_UI);
  
    //PROTECTED
    GameObject* protected_UI = new GameObject(PROTECTED_POS);
        Protected* protected_behaviour = new Protected(*protected_UI);
        CameraFollower *protected_UI_cmfl = new CameraFollower(*protected_UI);
        protected_UI->AddComponent((std::shared_ptr<CameraFollower>)protected_UI_cmfl);
        protected_UI->AddComponent(std::shared_ptr<Protected>(protected_behaviour));
        Game::GetInstance().GetCurrentState().AddObject(protected_UI);
 

    int offsetArray = 0; 
    int ofsArrayIn = 0; // offset distance between normal and Djinn array

    for (int i = SkillNormalArray.size() - 1; i >= 0; i--) {
        int offsetArray = SkillNormalArray.size() - i - 1;
        GameObject* normalSkill = new GameObject(SKILL_SPACE * offsetArray + SKILL_N_OFFSET.x, SKILL_N_OFFSET.y);
        // Acesse o Skill::SkillId a partir do std::shared_ptr<Skill>
        Skill::SkillId skillId = SkillNormalArray[i];

        Skill* skill_behaviour = new Skill(*normalSkill, skillId, ap_behaviour);
        CameraFollower *normalSkill_cmfl = new CameraFollower(*normalSkill);
        normalSkill->AddComponent((std::shared_ptr<CameraFollower>)normalSkill_cmfl);
        normalSkill->AddComponent(std::shared_ptr<Skill>(skill_behaviour));
        Game::GetInstance().GetCurrentState().AddObject(normalSkill); 
        ofsArrayIn = SKILL_SPACE * offsetArray;  
    }   

    offsetArray = 0;
    for (int i = SkillDjinArray.size() - 1; i >= 0; i--) {
        offsetArray = SkillDjinArray.size() - i - 1;
        GameObject* normalSkill = new GameObject(ofsArrayIn + SKILL_SPACE * (offsetArray + 1) + SKILL_N_OFFSET.x, SKILL_N_OFFSET.y);
        // Acesse o Skill::SkillId a partir do std::shared_ptr<Skill>
        Skill::SkillId skillId = SkillDjinArray[i];
 
        Skill* skill_behaviour = new Skill(*normalSkill, skillId, ap_behaviour); 
        CameraFollower *normalSkill_cmfl = new CameraFollower(*normalSkill);
        normalSkill->AddComponent((std::shared_ptr<CameraFollower>)normalSkill_cmfl);
        normalSkill->AddComponent(std::shared_ptr<Skill>(skill_behaviour));
        Game::GetInstance().GetCurrentState().AddObject(normalSkill);
        
    }
    

} 
 


UI::~UI() 
{
     
}



void UI::AddSkill(Skill::SkillId skill) {
    SkillNormalArray.push_back(skill);
}

void UI::Start() {  
    
    GameObject *ui = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    Sprite *ui_screen_spr = new Sprite(*ui, UI_SCREEN_SPRITE);
    CameraFollower *ui_cmfl = new CameraFollower(*ui);
    ui->AddComponent((std::shared_ptr<CameraFollower>)ui_cmfl);
    ui->AddComponent((std::shared_ptr<Sprite>)ui_screen_spr);
    Game::GetInstance().GetCurrentState().AddObject(ui);

}

void UI::Update(float dt) { 
 
}
 
void UI::Render() {

}




bool UI::Is(std::string type){
    return (type == "UI");
}

