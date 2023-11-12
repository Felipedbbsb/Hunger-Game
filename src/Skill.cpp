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
#include "SkillSelection.h"
#include "UI.h"

Skill* Skill::skillFromReward = nullptr; //selection reward  
Skill* Skill::skillToReward = nullptr; //selection reward  

Skill* Skill::selectedSkill = nullptr; //generic 

Skill* Skill::selectedSkillEnemy = nullptr; //generic 

Skill* Skill::skillBackToMother = nullptr; //back effects

Skill* Skill::skillBackToDaughter = nullptr; //back effects 

std::pair<bool, bool> Skill::HasTagRampageOrWeak;

std::map<Skill::SkillId, Skill::SkillInfo> Skill::skillInfoMap; // Defina o mapa

Skill::TargetType Skill::playerTargetType = Skill::IRR;

std::vector<Skill::SkillId> Skill::skillArray = {};
std::vector<std::weak_ptr<GameObject>> Skill::skillArrayObj = {};


Skill::Skill(GameObject& associated, SkillId id, AP* ap, bool createJewel)
    : Component::Component(associated),
    id(id),  
    readerSkill(nullptr),
    apInstance(ap),
    jewelObj(nullptr),
    tagCount(nullptr),
    toggleJewel(false),
    createJewel(createJewel) {
    
}

void Skill::Start() {
    std::string spriteSkill;
    // Use skillInfoMap para obter informações da habilidade com base na ID
    const SkillInfo& skillInfo = skillInfoMap[id];

    spriteSkill = skillInfo.iconPath;
    textSkill = skillInfo.info;
 
    Sprite* skillSprite = new Sprite(associated, spriteSkill);
    associated.AddComponent(std::make_shared<Sprite>(*skillSprite));

    if(jewelObj == nullptr && createJewel){
        jewelObj = new GameObject(associated.box.x, associated.box.y);
        Sprite* jewelObj_behaviour;
        if(skillInfo.apCost != 0){
            jewelObj_behaviour = new Sprite(*jewelObj, AP_FULL_SPRITE);
        }
        else{
            jewelObj_behaviour = new Sprite(*jewelObj, AP_EMPTY_SPRITE);
        }
        jewelObj->AddComponent(std::make_shared<Sprite>(*jewelObj_behaviour));
 
        jewelObj->box.x += associated.box.w - jewelObj->box.w/2;
        jewelObj->box.y += associated.box.h/2 - jewelObj->box.h/2;
        
        CameraFollower *jewelObj_cmfl = new CameraFollower(*jewelObj);
        jewelObj->AddComponent(std::make_shared<CameraFollower>(*jewelObj_cmfl));
        
        Game::GetInstance().GetCurrentState().AddObject(jewelObj);


        CreateTagCount();
    }

    
}

Skill::~Skill() {
    if(jewelObj != nullptr){
        jewelObj->RequestDelete();
        jewelObj = nullptr;
        if(tagCount){
            tagCount->RequestDelete();
            tagCount = nullptr;
        }
    }
}

void Skill::Update(float dt) {

    Vec2 mousePos(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());
    skillClickTimer.Update(dt);

    if (InputManager::GetInstance().MousePress(RIGHT_MOUSE_BUTTON) && GameData::playerTurn == true){
        if(selectedSkill){
            selectedSkill->Deselect();
        }
        
        skillToReward = nullptr;
        skillFromReward = nullptr;
    }

    if (associated.box.Contains(mousePos.x- Camera::pos.x, mousePos.y- Camera::pos.y)){ 

            if (skillClickTimer.Get() >= SKILL_CLICK_COOLDOWN) {
                if (!readerSkill) {
                    readerSkill = new GameObject(associated.box.x, associated.box.y);
                    Reader* readerSkill_behaviour = new Reader(*readerSkill, textSkill);
                    readerSkill->AddComponent(std::make_shared<Reader>(*readerSkill_behaviour));
                    Game::GetInstance().GetCurrentState().AddObject(readerSkill);


                }

                
                if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) && GameData::playerTurn == true) {
                    
                    //Scenerio where combat its not alerady on skill selection
                    if(!SkillSelection::skillSelectionActivated && (id != Skill::LOCKED1 && id != Skill::LOCKED2 && id != Skill::LOCKED3 && id != Skill::EMPTY)){
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

                    //Scenerio where combat its  alerady on skill selection
                    else if(SkillSelection::skillSelectionActivated && (id != Skill::LOCKED1 && id != Skill::LOCKED2 && id != Skill::LOCKED3 && id != Skill::EMPTY)){
                        if(createJewel){
                            if (skillToReward != nullptr && skillToReward != this  ) {
                                skillToReward = nullptr;
                            }
                            skillToReward = this;
                            
                        }else{
                            if (skillFromReward != nullptr && skillFromReward != this  ) {
                                skillFromReward = nullptr;
                            }
                            skillFromReward = this;
                        }
                        
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


 

    Skill::SkillInfo tempSkillInfo = skillInfoMap[id];
    bool available = (AP::apCount >= tempSkillInfo.apCost);
    if(SkillSelection::skillSelectionActivated){ // combat ended, no need for desaturation
        available = true;
    }


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
                    jewelObj->AddComponent(std::make_shared<Sprite>(*jewelObj_behaviour2));
                    toggleJewel = true;  
                    tagCount->RequestDelete();
                    tagCount = nullptr;
                }
            } else { 
                if (toggleJewel) {
                    jewelObj->RemoveComponent(spriteComponentPtr);
                    const SkillInfo& skillInfo = skillInfoMap[id];
                    Sprite* jewelObj_behaviour;
                    if(skillInfo.apCost != 0){
                        jewelObj_behaviour = new Sprite(*jewelObj, AP_FULL_SPRITE);
                    }
                    else{
                        jewelObj_behaviour = new Sprite(*jewelObj, AP_EMPTY_SPRITE);
                    }
                    jewelObj->AddComponent(std::make_shared<Sprite>(*jewelObj_behaviour));
                    toggleJewel = false;  
                    CreateTagCount();
                } 
            }
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
}

void Skill::CreateTagCount() {
    const SkillInfo& skillInfo = skillInfoMap[id];

    //numberCounter
    if(skillInfo.apCost != 0){
        if(tagCount == nullptr){
            tagCount =  new GameObject(jewelObj->box.x, jewelObj->box.y); //posicao foi no olho...
            std::string textNumber = std::to_string(skillInfo.apCost);
            Text* tagCountNumber_behaviour = new Text(*tagCount, TEXT_TAGCOUNT_FONT, 
                                                                TEXT_TAGCOUNT2_SIZE,
                                                                Text::OUTLINE3,
                                                                textNumber, 
                                                                TEXT_TAGCOUNT_FONT_COLOR,
                                                                0);   

            //Centralize
            if(skillInfo.apCost == 1){
                tagCount->box.x += jewelObj->box.w/2 - tagCount->box.w * 0.45 ;                                                 
                tagCount->box.y += jewelObj->box.h/2 - tagCount->box.h/2; 
            }  
            else if(skillInfo.apCost == 2){
                tagCount->box.x += jewelObj->box.w/2 - tagCount->box.w * 0.40 ;                                                 
                tagCount->box.y += jewelObj->box.h/2 - tagCount->box.h/2; 
            }
            else if(skillInfo.apCost == 3){
                tagCount->box.x += jewelObj->box.w/2 - tagCount->box.w * 0.30 ;                                                 
                tagCount->box.y += jewelObj->box.h/2 - tagCount->box.h/2;
            }


            tagCount->AddComponent(std::make_shared<Text>(*tagCountNumber_behaviour));

            CameraFollower *tagCountj_cmfl = new CameraFollower(*tagCount);
            tagCount->AddComponent(std::make_shared<CameraFollower>(*tagCountj_cmfl));

            Game::GetInstance().GetCurrentState().AddObject(tagCount);
        }
    }
    
}

Skill::SkillId Skill::GetId() {
    return id;
}

bool Skill::Is(std::string type) {
    return (type == "Skill");
}

void Skill::AddSkill(Skill::SkillId id, Skill::SkillId skillIdToChange) {
    // Find the first occurrence of EMPTY in skillArray
    auto it = std::find(skillArray.begin(), skillArray.end(),skillIdToChange);

    // Check if there is an empty slot available
    if (it != skillArray.end()) {
        // Replace the empty slot with the new skill
        *it = id;
    }

    UI::CreateSkillsGO(nullptr);
  
}

//Starter skills
void Skill::InitializeSkills() {
    //Adding skills
        skillArray.push_back(Skill::HnS);
        skillArray.push_back(Skill::EMPTY);
        skillArray.push_back(Skill::Rockabye);
        skillArray.push_back(Skill::Stinger);
        skillArray.push_back(Skill::EMPTY);

        skillArray.push_back(Skill::LOCKED1);
        skillArray.push_back(Skill::LOCKED2);
        skillArray.push_back(Skill::LOCKED3);

        
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

    //-------------MOTHER SKILLS-----------
    //Helmbreaker (2AP): Deal 3 damage; Apply 2 Vulnerable. 
    skillInfoMap[Helmbreaker] = {2, Skill::StateProtected::NOCHANGES,      3, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE}, 0, {},   NS_Helmbreaker, I_Helmbreaker, SPR_Helmbreaker,  ATTACK_INDIVIDUAL, MOTHER,        NONE, IRR};

    //Rockabye (1AP): Apply 1 Resilience to your daughter.
    skillInfoMap[Rockabye] =  {1, Skill::StateProtected::NOCHANGES,      0, {Tag::Tags::RESILIENCE}, 0, {},                          NS_Rockabye, I_Rockabye, SPR_Rockabye,           BUFF_INDIVIDUAL, MOTHER,          NONE, IRR };
 
    //Stinger (2AP): Deal 5 Damage to all enemies; Expose your daughter.
    skillInfoMap[Stinger] =  {2, Skill::StateProtected::EXPOSED,      5, {},                      0, {},                          NS_Stinger, I_Stinger, SPR_Stinger,               ATTACK_ALL, MOTHER,               NONE, IRR };
 
    //------------DAUGHTER SKILL------------
    //Hide and Seek (1AP): Apply 1 Dodge and 1 Vulnerable to Mother; Protect Daughter 
    skillInfoMap[HnS] =  {1, Skill::StateProtected::PROTECTED,       0, {Tag::Tags::DODGE, Tag::Tags::VULNERABLE},     0, {},   NS_HnS, I_HnS, SPR_HnS,                        BUFF_INDIVIDUAL, DAUGHTER,        NONE, IRR} ;

    //-----------DJINN SKILLS--------
    //Instant Regret (3AP): Deal 15 damage; Expose your daughter; Apply 1 Vulnerable to your daughter; Lose 7HP
    skillInfoMap[InstantRegret] = {3, Skill::StateProtected::EXPOSED,      15, {},     0, {Tag::Tags::VULNERABLE},                     NS_InstantRegret, I_InstantRegret, SPR_InstantRegret,          ATTACK_INDIVIDUAL, MOTHER,        DEBUFF_INDIVIDUAL, DAUGHTER} ;


    //--------LOCKED--------
    skillInfoMap[LOCKED1] = {0, Skill::StateProtected::NOCHANGES,   0, {},     0, {},     NS_LOCKED, I_LOCKED, SPR_LOCKED,          NONE, IRR,        NONE, IRR};
    skillInfoMap[LOCKED2] = {0, Skill::StateProtected::NOCHANGES,   0, {},     0, {},     NS_LOCKED2, I_LOCKED2, SPR_LOCKED2,          NONE, IRR,        NONE, IRR};
    skillInfoMap[LOCKED3] = {0, Skill::StateProtected::NOCHANGES,   0, {},     0, {},     NS_LOCKED3, I_LOCKED3, SPR_LOCKED3,          NONE, IRR,        NONE, IRR};
    
    //--------EMPTY
    skillInfoMap[EMPTY] = {0, Skill::StateProtected::NOCHANGES,   0, {},     0, {},     NS_EMPTY, I_EMPTY, SPR_EMPTY,          NONE, IRR,        NONE, IRR};
    

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
    skillInfoMap[E3_Skill1] = {0, Skill::StateProtected::NOCHANGES,   0, {Tag::Tags::CURSE, Tag::Tags::CURSE},     0, {},     NS_Generic, I_Generic, SPR_Generic,          DEBUFF_INDIVIDUAL, IRR,        NONE, IRR} ;

    //Apply Rampage in all allies 
    skillInfoMap[E3_Skill2] = {0, Skill::StateProtected::NOCHANGES,   0, {Tag::Tags::RAMPAGE},     0, {},     NS_Generic, I_Generic, SPR_Generic,          BUFF_ALL, IRR,       NONE, IRR} ;

    
} 
       