#include "UI.h"
#include "Game.h"
#include "Camera.h"
#include "AP.h"
#include "Protected.h"
#include "CameraFollower.h"

// speed já está sendo inicializado pelo construtor de Vec2
// UI.h
GameObject* UI::uiGO = nullptr;

UI::UI(GameObject &associated)
    : Component(associated){
    

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
 
    
    CreateSkillsGO(ap_behaviour);

    

} 
 


UI::~UI() 
{
    for (int i = Skill::skillArrayObj.size() - 1; i >= 0; i--) { //remove skills
            Skill::skillArrayObj.erase(Skill::skillArrayObj.begin() + i);
    }

}

void UI::CreateSkillsGO( AP* ap_behaviour) {

    if(uiGO!=nullptr){
        uiGO->RequestDelete();
        uiGO = nullptr;
    }

    uiGO = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    Sprite *ui_screen_spr = new Sprite(*uiGO, UI_SCREEN_SPRITE);
    CameraFollower *ui_cmfl = new CameraFollower(*uiGO);
    uiGO->AddComponent((std::shared_ptr<CameraFollower>)ui_cmfl);
    uiGO->AddComponent((std::shared_ptr<Sprite>)ui_screen_spr);

    //resets
    for (int i = Skill::skillArrayObj.size() - 1; i >= 0; i--) { //remove skills
        Skill::skillArrayObj.erase(Skill::skillArrayObj.begin() + i);
    }


    for (unsigned int i = 0; i < Skill::skillArray.size(); i++) {
        int offsetArray = i;
        GameObject* normalSkill = new GameObject(SKILL_SPACE * offsetArray + SKILL_N_OFFSET.x, uiGO->box.y + 123 );
        // Acesse o Skill::SkillId a partir do std::shared_ptr<Skill>
        Skill::SkillId skillId = Skill::skillArray[i];
        Skill* skill_behaviour = new Skill(*normalSkill, skillId, ap_behaviour);
        CameraFollower *normalSkill_cmfl = new CameraFollower(*normalSkill);
        normalSkill->AddComponent((std::shared_ptr<CameraFollower>)normalSkill_cmfl);
        normalSkill->AddComponent(std::shared_ptr<Skill>(skill_behaviour));
        auto weak_skill = Game::GetInstance().GetCurrentState().AddObject(normalSkill); 
        Skill::skillArrayObj.push_back(weak_skill);
    }  

    Game::GetInstance().GetCurrentState().AddObject(uiGO);

}


void UI::Start() {  
    
    uiGO = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    Sprite *ui_screen_spr = new Sprite(*uiGO, UI_SCREEN_SPRITE);
    CameraFollower *ui_cmfl = new CameraFollower(*uiGO);
    uiGO->AddComponent((std::shared_ptr<CameraFollower>)ui_cmfl);
    uiGO->AddComponent((std::shared_ptr<Sprite>)ui_screen_spr);
    Game::GetInstance().GetCurrentState().AddObject(uiGO);

}

void UI::Update(float dt) { 
 
} 
 
void UI::Render() {

}




bool UI::Is(std::string type){
    return (type == "UI");
}

