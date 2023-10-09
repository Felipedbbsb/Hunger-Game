#include "Skill.h"
#include "Reader.h"
#include "Camera.h"
#include "InputManager.h"
#include "Game.h"


Skill::Skill(GameObject &associated, SkillId id)
    : Component::Component(associated), 
    id(id),
    readerSkill(nullptr){
    // Add other initialization here
}

void Skill::Start() { 
    std::string spriteSkill; // Declare spriteSkill 

    switch (id) {
        case SkillId::SKILL1: 
            spriteSkill = SKILL1_SPRITE;
            textSkill = TEXT_SKILL1; 
            break; // Use "break" para sair do switch após cada case
 
        case SkillId::SKILL2: 
            spriteSkill = SKILL2_SPRITE;
            textSkill = TEXT_SKILL2;
            break; // Use "break" para sair do switch após cada case
        case SkillId::SKILL3:  
            spriteSkill = SKILL3_SPRITE;
            textSkill = TEXT_SKILL3;
            break; // Use "break" para sair do switch após cada case
 
        case SkillId::SKILL4: 
            spriteSkill = SKILL4_SPRITE;
            textSkill = TEXT_SKILL4;
            break; // Use "break" para sair do switch após cada case    
    }


    Sprite *skillSprite = new Sprite(associated, spriteSkill); // or SKILL2_SPRITE depending on the skill
    skillSprite->SetScale(0.2, 0.2);
    associated.AddComponent(std::shared_ptr<Sprite>(skillSprite)); 
}

Skill::~Skill()
{  
     
} 
 
void Skill::Update(float dt) {
    // Verify how long the mouse is on Skill and, after a cooldown, open the comment box
    Vec2 mousePos(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());
    skillClickTimer.Update(dt);
    
    if (associated.box.Contains(mousePos.x, mousePos.y)) {
        if (skillClickTimer.Get() >= SKILL_CLICK_COOLDOWN) {
            std::weak_ptr<GameObject> weak_skillRef = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);

            if (!readerSkill) { 
                // Create the readerSkill if it doesn't exist
                readerSkill = new GameObject(associated.box.x, associated.box.y);
                Reader* readerSkill_behaviour = new Reader(*readerSkill, weak_skillRef, textSkill);
                readerSkill->AddComponent(std::shared_ptr<Reader>(readerSkill_behaviour));
                Game::GetInstance().GetCurrentState().AddObject(readerSkill);
            }
        }
    } else {
        skillClickTimer.Restart();  
        if (readerSkill) {
            readerSkill->RequestDelete(); // Delete the readerSkill if it exists
            readerSkill = nullptr; // Set it to nullptr to avoid further usage
        }
    }
}

    

void Skill::Render()
{ 
}
 
Skill::SkillId Skill::GetId(){
    return id; 
}

bool Skill::Is(std::string type){
    return (type == "Skill");
}

