#include "Skill.h"
#include "Reader.h"
#include "Camera.h"
#include "InputManager.h"
#include "Game.h"
#include "Tag.h"
#include "AP.h"
 
Skill* Skill::selectedSkill = nullptr;

std::map<Skill::SkillId, Skill::SkillInfo> Skill::skillInfoMap; // Defina o mapa

Skill::Skill(GameObject& associated, SkillId id, AP* ap)
    : Component::Component(associated),
    id(id),  
    readerSkill(nullptr),
    apInstance(ap) {
}

void Skill::Start() {
    std::string spriteSkill;
    // Use skillInfoMap para obter informações da habilidade com base na ID
    const SkillInfo& skillInfo = skillInfoMap[id];

    spriteSkill = skillInfo.iconPath;
    textSkill = skillInfo.info;
 
    Sprite* skillSprite = new Sprite(associated, spriteSkill);
    associated.AddComponent(std::shared_ptr<Sprite>(skillSprite));
}

Skill::~Skill() {
}

void Skill::Update(float dt) {
    Vec2 mousePos(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());
    skillClickTimer.Update(dt);

    if (InputManager::GetInstance().MousePress(RIGHT_MOUSE_BUTTON) && selectedSkill){
                selectedSkill->Deselect();
    }

    if (associated.box.Contains(mousePos.x, mousePos.y)) { 
        if (skillClickTimer.Get() >= SKILL_CLICK_COOLDOWN) {
            if (!readerSkill) {
                readerSkill = new GameObject(associated.box.x, associated.box.y);
                Reader* readerSkill_behaviour = new Reader(*readerSkill, textSkill);
                readerSkill->AddComponent(std::shared_ptr<Reader>(readerSkill_behaviour));
                Game::GetInstance().GetCurrentState().AddObject(readerSkill);


            }
            if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) {

                if (selectedSkill != nullptr && selectedSkill != this  ) {
                    selectedSkill->Deselect();
                }
                selectedSkill = this;

                //Checks if apCount is bigger or equal to apCost
                Skill::SkillInfo tempSkillInfo = skillInfoMap[selectedSkill->GetId()];
                // Check if there are enough AP points to select the skill
                if (AP::apCount >= tempSkillInfo.apCost) {
                    // Deduct the AP cost from the total AP count
                    apInstance->MirrorAPCount(AP::apCount - tempSkillInfo.apCost);
                } else {
                    selectedSkill->Deselect();
                }


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
    apInstance->SetAPCount(AP::apCount);
}

void Skill::Render() {
    Skill::SkillInfo tempSkillInfo = skillInfoMap[id];
    bool available = (AP::apCount >= tempSkillInfo.apCost);

    if (!available) {
        // Apply the desaturation effect
        auto spriteComponent = associated.GetComponent("Sprite");
        if (spriteComponent) {
            //spriteComponent->SetDesaturation(true);
        }
    } else {
        // Ensure the sprite is not desaturated
        auto spriteComponent = associated.GetComponent("Sprite");
        if (spriteComponent) {
            //spriteComponent->SetDesaturation(false);
        }
    }
}

Skill::SkillId Skill::GetId() {
    return id;
}

bool Skill::Is(std::string type) {
    return (type == "Skill");
}

void Skill::InitializeSkillInfoMap() {
    //struct SkillInfo {
    //    ap cost;     
    //    damage; 
    //    tags;
    //    name;
    //    iconPath;
    //};
    // Populate the map with skill information during initialization.
    //Use for example               Skill::SkillInfo tempSkillInfo = skillInfoMap[selectedSkill->GetId()];
    //                              tempSkillInfo.damage to catch damage by the id

    skillInfoMap[SKILL1] = {2, 5, {Tag::Tags::VULNERABLE}, NAME_SKILL1, INFO_SKILL1, SKILL1_SPRITE, ATTACK_ALL};
    skillInfoMap[SKILL2] = {3, 20, {Tag::Tags::RESILIENCE}, NAME_SKILL2, INFO_SKILL2, SKILL2_SPRITE, ATTACK_INDIVIDUAL};
    skillInfoMap[SKILL3] = {1, 5, {Tag::Tags::DODGE}, NAME_SKILL3, INFO_SKILL3, SKILL3_SPRITE, DEFENSE_INDIVIDUAL};
    skillInfoMap[SKILL4] = {1, 5, {}, NAME_SKILL4, INFO_SKILL4, SKILL4_SPRITE, DEFENSE_ALL};

}
      