#include "Skill.h"
#include "Reader.h"
#include "Camera.h"
#include "InputManager.h"
#include "Game.h"
#include "Tag.h"
#include "AP.h"
#include "Text.h"
#include "GameData.h"
#include "Camera.h"
#include "CameraFollower.h"
 
Skill* Skill::selectedSkill = nullptr; //generic 

Skill* Skill::selectedSkillEnemy = nullptr; //generic 

Skill* Skill::skillBackToMother = nullptr; //back effects

Skill* Skill::skillBackToDaughter = nullptr; //back effects 

std::pair<bool, bool> Skill::HasTagRampageOrWeak;

std::map<Skill::SkillId, Skill::SkillInfo> Skill::skillInfoMap; // Defina o mapa

Skill::TargetType Skill::playerTargetType = Skill::IRR;


Skill::Skill(GameObject& associated, SkillId id, AP* ap)
    : Component::Component(associated),
    id(id),  
    readerSkill(nullptr),
    apInstance(ap),
    jewelObj(nullptr),
    toggleJewel(false) {

    //Game::GetInstance().GetCurrentState().AddObject(&associated); 

    
}

void Skill::Start() {
    std::string spriteSkill;
    // Use skillInfoMap para obter informações da habilidade com base na ID
    const SkillInfo& skillInfo = skillInfoMap[id];

    spriteSkill = skillInfo.iconPath;
    textSkill = skillInfo.info;
 
    Sprite* skillSprite = new Sprite(associated, spriteSkill);
    associated.AddComponent(std::shared_ptr<Sprite>(skillSprite));

    if(jewelObj == nullptr){
        jewelObj = new GameObject(associated.box.x, associated.box.y);
        Sprite* jewelObj_behaviour = new Sprite(*jewelObj, AP_FULL_SPRITE);
        jewelObj->AddComponent(std::shared_ptr<Sprite>(jewelObj_behaviour));

        jewelObj->box.x += associated.box.w - jewelObj->box.w/2;
        jewelObj->box.y += associated.box.h/2 - jewelObj->box.h/2;
        
        CameraFollower *jewelObj_cmfl = new CameraFollower(*jewelObj);
        jewelObj->AddComponent((std::shared_ptr<CameraFollower>)jewelObj_cmfl);
        
        Game::GetInstance().GetCurrentState().AddObject(jewelObj);
        
    }

    
}

Skill::~Skill() {
    if(jewelObj != nullptr){
        jewelObj->RequestDelete();
        jewelObj = nullptr;
    }
}

void Skill::Update(float dt) {

    Vec2 mousePos(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());
    skillClickTimer.Update(dt);

    if (InputManager::GetInstance().MousePress(RIGHT_MOUSE_BUTTON) && selectedSkill && GameData::playerTurn == true){
                selectedSkill->Deselect();
    }

    if (associated.box.Contains(mousePos.x- Camera::pos.x, mousePos.y- Camera::pos.y)) { 
        if (skillClickTimer.Get() >= SKILL_CLICK_COOLDOWN) {
            if (!readerSkill) {
                readerSkill = new GameObject(associated.box.x, associated.box.y);
                Reader* readerSkill_behaviour = new Reader(*readerSkill, textSkill);
                readerSkill->AddComponent(std::shared_ptr<Reader>(readerSkill_behaviour));
                Game::GetInstance().GetCurrentState().AddObject(readerSkill);


            }
            if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) && GameData::playerTurn == true) {
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





//Rebound skill
void Skill::SkillBack(TargetType targetTypeBack){
    if(targetTypeBack == TargetType::MOTHER){
        skillBackToMother = selectedSkill;
    }

    if(targetTypeBack == TargetType::DAUGHTER){
        skillBackToDaughter = selectedSkill;
    }
    
}

void Skill::DeselectBack(TargetType targetTypeBack) {
    if(targetTypeBack == TargetType::MOTHER){
        skillBackToMother = nullptr;
    }

    if(targetTypeBack == TargetType::DAUGHTER){
        skillBackToDaughter = nullptr;
    }
}




void Skill::Render() {
    Skill::SkillInfo tempSkillInfo = skillInfoMap[id];
    bool available = (AP::apCount >= tempSkillInfo.apCost);

    auto spriteComponent = associated.GetComponent("Sprite");
    auto spriteComponentPtr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
    if (spriteComponentPtr) {
        if (!available) {
            // Apply the desaturation effect
            spriteComponentPtr->SetDesaturation(true);
        } else {
            // Ensure the sprite is not desaturated
            spriteComponentPtr->SetDesaturation(false);
        }
    }    

    if(jewelObj != nullptr){
        spriteComponent = jewelObj->GetComponent("Sprite");
        spriteComponentPtr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
        if (spriteComponentPtr) {
            if (!available) {
                if (!toggleJewel) {
                    jewelObj->RemoveComponent(spriteComponentPtr);
                    Sprite* jewelObj_behaviour2 = new Sprite(*jewelObj, AP_EMPTY_SPRITE);
                    jewelObj->AddComponent(std::shared_ptr<Sprite>(jewelObj_behaviour2));
                    toggleJewel = true;  
                }
            } else { 
                if (toggleJewel) {
                    jewelObj->RemoveComponent(spriteComponentPtr);
                    Sprite* jewelObj_behaviour3 = new Sprite(*jewelObj, AP_FULL_SPRITE);
                    jewelObj->AddComponent(std::shared_ptr<Sprite>(jewelObj_behaviour3));
                    toggleJewel = false;  
                }
            }
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

    skillInfoMap[SKILL1] = {2, Skill::StateProtected::NOCHANGES, 5, {Tag::Tags::VULNERABLE}, 0, {}, NAME_SKILL1, INFO_SKILL1, SKILL1_SPRITE, ATTACK_ALL, IRR, ATTACK_ALL, IRR};
    skillInfoMap[SKILL2] = {2, Skill::StateProtected::NOCHANGES, 0, {Tag::Tags::RESILIENCE}, 0, {}, NAME_SKILL2, INFO_SKILL2, SKILL2_SPRITE, ATTACK_INDIVIDUAL, DAUGHTER, ATTACK_ALL, IRR};
    skillInfoMap[SKILL3] = {1, Skill::StateProtected::NOCHANGES, 0, {Tag::Tags::DODGE}, 0, {}, NAME_SKILL3, INFO_SKILL3, SKILL3_SPRITE, DEFENSE_INDIVIDUAL, IRR, ATTACK_ALL, IRR};
    skillInfoMap[SKILL4] = {1, Skill::StateProtected::NOCHANGES, 5, {}, 5, {}, NAME_SKILL4, INFO_SKILL4, SKILL4_SPRITE, ATTACK_INDIVIDUAL, MOTHER, ATTACK_ALL, IRR};

    //-------------MOTHER SKILLS-----------
    //Helmbreaker (2AP): Deal 3 damage; Apply 2 Vulnerable. 
    skillInfoMap[Helmbreaker] = {2, Skill::StateProtected::NOCHANGES,      3, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE}, 0, {},   NS_Helmbreaker, I_Helmbreaker, SPR_Helmbreaker,  ATTACK_INDIVIDUAL, MOTHER,        NONE, IRR};

    //Rockabye (2AP): Apply 1 Resilience to your daughter.
    skillInfoMap[Rockabye] =  {2, Skill::StateProtected::NOCHANGES,      0, {Tag::Tags::RESILIENCE}, 0, {},                          NS_Rockabye, I_Rockabye, SPR_Rockabye,           BUFF_INDIVIDUAL, MOTHER,          NONE, IRR };
 
    //Stinger (2AP): Deal 5 Damage to all enemies; Expose your daughter.
    skillInfoMap[Stinger] =  {2, Skill::StateProtected::EXPOSED,      5, {},                      0, {},                          NS_Stinger, I_Stinger, SPR_Stinger,               ATTACK_ALL, MOTHER,               NONE, IRR };
 
    //------------DAUGHTER SKILL------------
    //Hide and Seek (1AP): Apply 1 Dodge and 1 Vulnerable to Mother; Protect Daughter 
    skillInfoMap[HnS] =  {1, Skill::StateProtected::PROTECTED,       0, {Tag::Tags::DODGE, Tag::Tags::VULNERABLE},     0, {},   NS_HnS, I_HnS, SPR_HnS,                        BUFF_INDIVIDUAL, DAUGHTER,        NONE, IRR} ;

    //-----------DJINN SKILLS--------
    //Instant Regret (3AP): Deal 15 damage; Expose your daughter; Apply 1 Vulnerable to your daughter; Lose 7HP
    skillInfoMap[InstantRegret] = {3, Skill::StateProtected::EXPOSED,      15, {},     0, {Tag::Tags::VULNERABLE},                     NS_InstantRegret, I_InstantRegret, SPR_InstantRegret,          ATTACK_INDIVIDUAL, MOTHER,        DEBUFF_INDIVIDUAL, DAUGHTER} ;


    //----------Enemies skill------------
    // Deal 5 damage.
    skillInfoMap[E1_Skill1] = {0, Skill::StateProtected::NOCHANGES,   5, {},     0, {},     NS_Generic, I_Generic, SPR_Generic,          ATTACK_INDIVIDUAL, IRR,        NONE, IRR} ;

    //Deal 2 damage; Gain 2 Rampage.
    skillInfoMap[E1_Skill2] = {0, Skill::StateProtected::NOCHANGES,   2, {},     0, {Tag::Tags::RAMPAGE, Tag::Tags::RAMPAGE},     NS_Generic, I_Generic, SPR_Generic,          ATTACK_INDIVIDUAL, IRR,       BUFF_INDIVIDUAL, IRR} ;

    //Deal 7 Damage.
    skillInfoMap[E1_Skill3] = {0, Skill::StateProtected::NOCHANGES,   7, {},     0, {},     NS_Generic, I_Generic, SPR_Generic,          ATTACK_INDIVIDUAL, IRR,        NONE, IRR} ;


    // Skill 1: Deal 1 damage; Apply 1 Weak to target
    skillInfoMap[E2_Skill1] = {0, Skill::StateProtected::NOCHANGES,   1, {Tag::Tags::WEAK, Tag::Tags::WEAK},     0, {},     NS_Generic, I_Generic, SPR_Generic,          ATTACK_INDIVIDUAL, IRR,        NONE, IRR} ;

    //Apply 1 Resilience to a random ally
    skillInfoMap[E2_Skill2] = {0, Skill::StateProtected::NOCHANGES,   0, {Tag::Tags::RESILIENCE},     0, {},     NS_Generic, I_Generic, SPR_Generic,          BUFF_INDIVIDUAL, IRR,       BUFF_INDIVIDUAL, IRR} ;

    // Apply 1 Weak to all enemies
    skillInfoMap[E2_Skill3] = {0, Skill::StateProtected::NOCHANGES,   5, {Tag::Tags::WEAK},     0, {},     NS_Generic, I_Generic, SPR_Generic,          ATTACK_INDIVIDUAL, IRR,        NONE, IRR} ;


    // Apply 2 vulnerable 
    skillInfoMap[E3_Skill1] = {0, Skill::StateProtected::NOCHANGES,   8, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE},     0, {},     NS_Generic, I_Generic, SPR_Generic,          ATTACK_INDIVIDUAL, IRR,        NONE, IRR} ;

    //Apply Rampage in all allies
    skillInfoMap[E3_Skill2] = {0, Skill::StateProtected::NOCHANGES,   0, {Tag::Tags::RAMPAGE},     0, {},     NS_Generic, I_Generic, SPR_Generic,          BUFF_ALL, IRR,       NONE, IRR} ;

 
} 
       