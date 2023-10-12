#include "Skill.h"
#include "Reader.h"
#include "Camera.h"
#include "InputManager.h"
#include "Game.h"
 
Skill* Skill::selectedSkill = nullptr;

std::map<Skill::SkillId, Skill::SkillInfo> Skill::skillInfoMap; // Defina o mapa

Skill::Skill(GameObject& associated, SkillId id)
    : Component::Component(associated),
    id(id), 
    readerSkill(nullptr) {
}

void Skill::Start() {
    std::string spriteSkill;
    // Use skillInfoMap para obter informações da habilidade com base na ID
    const SkillInfo& skillInfo = skillInfoMap[id];

    spriteSkill = skillInfo.iconPath;
    textSkill = skillInfo.info;

    Sprite* skillSprite = new Sprite(associated, spriteSkill);
    skillSprite->SetScale(0.2, 0.2);
    associated.AddComponent(std::shared_ptr<Sprite>(skillSprite));
}

Skill::~Skill() {
}

void Skill::Update(float dt) {
    Vec2 mousePos(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());
    skillClickTimer.Update(dt);

    if (associated.box.Contains(mousePos.x, mousePos.y)) {
        if (skillClickTimer.Get() >= SKILL_CLICK_COOLDOWN) {
            std::weak_ptr<GameObject> weak_skillRef = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);

            if (!readerSkill) {
                readerSkill = new GameObject(associated.box.x, associated.box.y);
                Reader* readerSkill_behaviour = new Reader(*readerSkill, weak_skillRef, textSkill);
                readerSkill->AddComponent(std::shared_ptr<Reader>(readerSkill_behaviour));
                Game::GetInstance().GetCurrentState().AddObject(readerSkill);
            }
            if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) {
                if (selectedSkill != nullptr && selectedSkill != this) {
                    
                    Skill::SkillInfo tempSkillInfo = skillInfoMap[selectedSkill->GetId()];

                    
                    selectedSkill->Deselect();
                }
                selectedSkill = this; 
                //std::cout <<"Selecionado" << selectedSkill << std::endl;
            }
        }
    } else {
        skillClickTimer.Restart();
        if (readerSkill) {
            readerSkill->RequestDelete();
            readerSkill = nullptr;
        } 
    }
} 


void Skill::Deselect() {
    selectedSkill = nullptr;
    if (readerSkill != nullptr) {
        readerSkill->RequestDelete(); 
        readerSkill = nullptr;
    }
}

void Skill::Render() {
}

Skill::SkillId Skill::GetId() {
    return id;
}

bool Skill::Is(std::string type) {
    return (type == "Skill");
}

void Skill::InitializeSkillInfoMap() {
    //struct SkillInfo {
    //    damage; 
    //    tags;
    //    name;
    //    iconPath;
    //};
    // Populate the map with skill information during initialization.
    //Use for example               Skill::SkillInfo tempSkillInfo = skillInfoMap[selectedSkill->GetId()];
    //                              tempSkillInfo.damage to catch damage by the id

    skillInfoMap[SKILL1] = {5, {VULNERABLE, WEAK}, NAME_SKILL1, INFO_SKILL1, SKILL1_SPRITE, ATTACK_ALL};
    skillInfoMap[SKILL2] = {20, {RAMPAGE}, NAME_SKILL2, INFO_SKILL2, SKILL2_SPRITE, ATTACK_INDIVIDUAL};
    skillInfoMap[SKILL3] = {10, {RESILIENCE}, NAME_SKILL3, INFO_SKILL3, SKILL3_SPRITE, DEFENSE_INDIVIDUAL};
    skillInfoMap[SKILL4] = {20, {}, NAME_SKILL4, INFO_SKILL4, SKILL4_SPRITE, DEFENSE_ALL};

}
 