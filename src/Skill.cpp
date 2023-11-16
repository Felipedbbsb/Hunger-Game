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
    createJewel(createJewel),
    skillSelected(nullptr) {

    std::string spriteSkill;
    // Use skillInfoMap para obter informações da habilidade com base na ID
    const SkillInfo& skillInfo = skillInfoMap[id];

    spriteSkill = skillInfo.iconPath;
    textSkill = skillInfo.info;
 
    Sprite* skillSprite = new Sprite(associated, spriteSkill);
    associated.AddComponent(std::make_shared<Sprite>(*skillSprite));
    
}

void Skill::Start() {
    std::string spriteSkill;
    // Use skillInfoMap para obter informações da habilidade com base na ID
    const SkillInfo& skillInfo = skillInfoMap[id];



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

    //Creates border to sinalize selection of skill
    if(Skill::skillFromReward == this || Skill::skillToReward == this || Skill::selectedSkill == this){
        
        if(skillSelected == nullptr){
            skillSelected = new GameObject(associated.box.x + Camera::pos.x - 4, associated.box.y + Camera::pos.y -2);
            Sprite* skillSelected_behaviour = new Sprite(*skillSelected, SKILL_SELECTED_OBJ);
            skillSelected->AddComponent(std::make_shared<Sprite>(*skillSelected_behaviour));

            CameraFollower *skillSelected_cmfl = new CameraFollower(*skillSelected);
            skillSelected->AddComponent(std::make_shared<CameraFollower>(*skillSelected_cmfl));

            Game::GetInstance().GetCurrentState().AddObject(skillSelected); 

            if(readerSkill != nullptr){
                readerSkill->RequestDelete();
                readerSkill = nullptr;
            }
        }
           

    }
    else{
        if(skillSelected != nullptr){
            skillSelected->RequestDelete();
            skillSelected = nullptr;
        }
    }

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
                    readerSkill = new GameObject(associated.box.x + Camera::pos.x, associated.box.y + Camera::pos.y);
                    Reader* readerSkill_behaviour = new Reader(*readerSkill, textSkill);
                    readerSkill->AddComponent(std::make_shared<Reader>(*readerSkill_behaviour));

                    CameraFollower *readerSkill_cmfl = new CameraFollower(*readerSkill);
                    readerSkill->AddComponent(std::make_shared<CameraFollower>(*readerSkill_cmfl));

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

                            //Scenerio where skills its djiin style, rules you can only choose normal skills to change
                            if(SkillSelection::selectionSkillDjinnStyle){
                                //TODO
                            }
                            //Scenerio where skills it not djiin style
                            else{
                                skillToReward = this;

                            }
                            
                            
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
                    BlankTagCount(true);
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
                    BlankTagCount(false);
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

void Skill::BlankTagCount(bool isBlank) {
    const SkillInfo& skillInfo = skillInfoMap[id];
    if(skillInfo.apCost != 0){
        if(tagCount != nullptr && !isBlank){
            auto textComponent = tagCount->GetComponent("Text");
            auto textComponentPtr = std::dynamic_pointer_cast<Text>(textComponent);
            if(textComponentPtr){
                tagCount->RemoveComponent(textComponentPtr);
            }
            std::string textNumber = std::to_string(skillInfo.apCost);
            Text* tagCountNumber_behaviour = new Text(*tagCount, TEXT_TAGCOUNT_FONT, 
                                                                 TEXT_TAGCOUNT2_SIZE,
                                                                 Text::OUTLINE3,
                                                                 textNumber, 
                                                                 TEXT_TAGCOUNT_FONT_COLOR,
                                                                 0);  
            tagCount->AddComponent(std::make_shared<Text>(*tagCountNumber_behaviour));                                                                 
            
        }
        else if(tagCount != nullptr && isBlank){
            auto textComponent = tagCount->GetComponent("Text");
            auto textComponentPtr = std::dynamic_pointer_cast<Text>(textComponent);
            if(textComponentPtr){
                tagCount->RemoveComponent(textComponentPtr);
            }    
        } 
    }
}


void Skill::CreateTagCount() {
    const SkillInfo& skillInfo = skillInfoMap[id];

    //numberCounter
    if(skillInfo.apCost != 0){
        if(tagCount == nullptr){ 
            tagCount =  new GameObject(jewelObj->box.x , jewelObj->box.y); //posicao foi no olho...
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
        skillArray.push_back(Skill::RiskyManeuver);
        skillArray.push_back(Skill::InstantRegret);
        skillArray.push_back(Skill::Rockabye);
        skillArray.push_back(Skill::AMillionStabs);

        skillArray.push_back(Skill::DanseMacabre);
        skillArray.push_back(Skill::LOCKED1);
        skillArray.push_back(Skill::LOCKED2);

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

    //==================================MOTHER SKILLS==================================
    //Helmbreaker (2AP): Deal 6 damage; Apply 2 Vulnerable. 
    skillInfoMap.insert({Helmbreaker, {2, Skill::StateProtected::NOCHANGES,      6, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE}, 0, {},   NS_Helmbreaker, I_Helmbreaker, SPR_Helmbreaker,  ATTACK_INDIVIDUAL, MOTHER,        NONE, IRR}});

    //Rockabye (1AP): Apply 1 Resilience to your daughter.
    skillInfoMap[Rockabye] =  {1, Skill::StateProtected::NOCHANGES,      0, {Tag::Tags::RESILIENCE}, 0, {},                          NS_Rockabye, I_Rockabye, SPR_Rockabye,           BUFF_INDIVIDUAL, MOTHER,          NONE, IRR };
 
    //Stinger (2AP): Deal 6 Damage to all enemies; Expose your daughter.
    skillInfoMap[Stinger] =  {2, Skill::StateProtected::EXPOSED,      6, {},                      0, {},                          NS_Stinger, I_Stinger, SPR_Stinger,               ATTACK_ALL, MOTHER,               NONE, IRR };
 
    //Reckless Slash (2AP):Deal 6 damage; Expose your daughter
    skillInfoMap[RecklessSlash] =  {2, Skill::StateProtected::EXPOSED,      6, {},                      0, {},                          NS_RecklessSlash, I_RecklessSlash, SPR_RecklessSlash,               ATTACK_INDIVIDUAL, MOTHER,               NONE, IRR };
    
    //Cautious Strike (2AP): Deal 3 damage; Protect your daughter.
    skillInfoMap[CautiousStrike] =  {2, Skill::StateProtected::PROTECTED,      3, {},                      0, {},                          NS_CautiousStrike, I_CautiousStrike, SPR_CautiousStrike,               ATTACK_INDIVIDUAL, MOTHER,               NONE, IRR };
    
    //Motherly Love (1AP): Apply Provoke to Target; Gain 1 resilience;
    skillInfoMap[MotherlyLove] =  {1, Skill::StateProtected::NOCHANGES,      0, {Tag::Tags::PROVOKE},                      0, {Tag::Tags::RESILIENCE},                          NS_MotherlyLove, I_MotherlyLove, SPR_MotherlyLove,               DEBUFF_INDIVIDUAL, MOTHER,               BUFF_INDIVIDUAL, MOTHER };
    

    //Pommel Strike (2AP): Deal 6 Damage; Gain 1 Resilience.
    skillInfoMap[PommelStrike] =  {2, Skill::StateProtected::NOCHANGES,      6, {},                      0, {Tag::Tags::RESILIENCE},                          NS_PommelStrike, I_PommelStrike, SPR_PommelStrike,               ATTACK_INDIVIDUAL, MOTHER,               BUFF_INDIVIDUAL, MOTHER };
    
    //Hyper Protective (2AP): Protect your daughter; Apply 2 weak.
    skillInfoMap[HyperProtective] =  {2, Skill::StateProtected::PROTECTED,      0, {Tag::Tags::WEAK, Tag::Tags::WEAK},              0, {},                          NS_HyperProtective, I_HyperProtective, SPR_HyperProtective,               DEBUFF_INDIVIDUAL, MOTHER,               NONE, IRR };

    //Risky Maneuver (3AP): Deal 10 damage; Gain 2 Rampage; Expose your daughter; 
    skillInfoMap[RiskyManeuver] =  {3, Skill::StateProtected::EXPOSED,      10, {},              0, {Tag::Tags::RAMPAGE, Tag::Tags::RAMPAGE},                          NS_RiskyManeuver, I_RiskyManeuver, SPR_RiskyManeuver,               ATTACK_INDIVIDUAL, MOTHER,               BUFF_INDIVIDUAL, MOTHER };

    //Battle Hymn (2AP): Apply 1 resilience and 1 dodge to daughter.
    skillInfoMap[BattleHymn] =  {2, Skill::StateProtected::NOCHANGES,      0, {Tag::Tags::RESILIENCE, Tag::Tags::DODGE},              0, {},                          NS_BattleHymn, I_BattleHymn, SPR_BattleHymn,               BUFF_INDIVIDUAL, MOTHER,               NONE, IRR };

    //Swift Footed (2AP):  Gain 3 Dodge; Expose your daughter.
    skillInfoMap[SwiftFooted] =  {2, Skill::StateProtected::EXPOSED,      0, {},              0, {Tag::Tags::DODGE, Tag::Tags::DODGE, Tag::Tags::DODGE},                          NS_SwiftFooted, I_SwiftFooted, SPR_SwiftFooted,               BUFF_INDIVIDUAL, MOTHER,               BUFF_INDIVIDUAL, MOTHER };
 
    //Blinded by Fear (1AP): Deal 4 Damage; Apply Dodge to target.
    skillInfoMap[BlindedbyFear] =  {1, Skill::StateProtected::NOCHANGES,      4, {Tag::Tags::DODGE},              0, {},                          NS_BlindedbyFear, I_BlindedbyFear, SPR_BlindedbyFear,               ATTACK_INDIVIDUAL, MOTHER,               NONE, IRR };

    //Solitude (1AP): Expose your daughter; Gain 2 Resilience
    skillInfoMap[Solitude] =  {1, Skill::StateProtected::EXPOSED,      0, {},              0, {Tag::Tags::RESILIENCE, Tag::Tags::RESILIENCE},                          NS_Solitude, I_Solitude, SPR_Solitude,               BUFF_INDIVIDUAL, MOTHER,               BUFF_INDIVIDUAL, MOTHER  };

    //Maternal Instincts (3AP): Protect your daughter; Gain 3 Rampage
    skillInfoMap[MaternalInstincts] =  {3, Skill::StateProtected::PROTECTED,      0, {},              0, {Tag::Tags::RAMPAGE, Tag::Tags::RAMPAGE, Tag::Tags::RAMPAGE},                          NS_MaternalInstincts, I_MaternalInstincts, SPR_MaternalInstincts,               BUFF_INDIVIDUAL, MOTHER,               BUFF_INDIVIDUAL, MOTHER  };

    //==================================DAUGHTER SKILL==================================
    //Pocket Sand [filha] (2AP): Deal 5 damage; Apply 1 weak
    skillInfoMap[PocketSand] =  {2, Skill::StateProtected::NOCHANGES,       5, {Tag::Tags::WEAK},     0, {},   NS_PocketSand, I_PocketSand, SPR_PocketSand,        ATTACK_INDIVIDUAL, DAUGHTER,        NONE, IRR} ;

    //Nana Nana Na [filha] (1AP): Apply 1 Vulnerable and 1 Weak to one enemy
    skillInfoMap[NanaNanaNa] =  {1, Skill::StateProtected::NOCHANGES,       0, {Tag::Tags::VULNERABLE, Tag::Tags::WEAK},     0, {},   NS_NanaNanaNa, I_NanaNanaNa, SPR_NanaNanaNa,        DEBUFF_INDIVIDUAL, DAUGHTER,        NONE, IRR} ;
    
    //Trick or Treat (1AP): Deal 1 damage; Gain 1 Dodge; Expose Daughter
    skillInfoMap[TrickorTreat] =  {1, Skill::StateProtected::EXPOSED,       1, {},     0, {Tag::Tags::DODGE},   NS_TrickorTreat, I_TrickorTreat, SPR_TrickorTreat,                        ATTACK_INDIVIDUAL, DAUGHTER,        BUFF_INDIVIDUAL, DAUGHTER} ;

    //Hide and Seek (1AP): Apply 1 Dodge and 1 Vulnerable to Mother; Protect Daughter 
    skillInfoMap[HnS] =  {1, Skill::StateProtected::PROTECTED,       0, {Tag::Tags::DODGE, Tag::Tags::VULNERABLE},     0, {},   NS_HnS, I_HnS, SPR_HnS,                        BUFF_INDIVIDUAL, DAUGHTER,        NONE, IRR} ;

    //Sweet Treats (2AP): Apply 2 rampage and 2 resilience to mother; Expose Daughter
    skillInfoMap[SweetTreats] =  {2, Skill::StateProtected::EXPOSED,       0, {Tag::Tags::RAMPAGE, Tag::Tags::RAMPAGE, Tag::Tags::RESILIENCE, Tag::Tags::RESILIENCE},     0, {},   NS_SweetTreats, I_SweetTreats, SPR_SweetTreats,                        BUFF_INDIVIDUAL, DAUGHTER,        NONE, IRR} ;

    //Around the Rosie (1AP): Apply 1 Resilience and 1 Rampage to mother; Expose Daughter;
    skillInfoMap[AroundtheRosie] =  {1, Skill::StateProtected::EXPOSED,       0, {Tag::Tags::RAMPAGE, Tag::Tags::RESILIENCE},     0, {},   NS_AroundtheRosie, I_AroundtheRosie, SPR_AroundtheRosie,                        BUFF_INDIVIDUAL, DAUGHTER,        NONE, IRR} ;

    //TAG! You’re It (2AP): Deal 1 damage; Apply 3 Vulnerable; Expose Daughter
    skillInfoMap[TagYoureIt] =  {2, Skill::StateProtected::EXPOSED,       1, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE},     0, {},   NS_TagYoureIt, I_TagYoureIt, SPR_TagYoureIt,                        ATTACK_INDIVIDUAL, DAUGHTER,        NONE, IRR} ;

    //Red Light (2AP): Protect Daughter; Deal 1 damage to all enemies; Apply 2 Weak to all enemies
    //Green Light (2AP): Expose Daughter; Deal 1 damage to all enemies; Apply 2 Vulnerable to all enemies
    //Temper Tantrum (1AP): Expose Daughter; Gain 1 Rampage; Deal 3 Damage
    //Desperate Flailing (1AP): Expose Daughter; Deal 2 damage to all enemies


    //==================================DJINN SKILLS==================================
    //Instant Regret (3AP): Deal 20 damage; Expose your daughter; Apply 1 Vulnerable to your daughter; Lose 7HP
    skillInfoMap[InstantRegret] = {3, Skill::StateProtected::EXPOSED,      20, {},     7, {Tag::Tags::VULNERABLE},                     NS_InstantRegret, I_InstantRegret, SPR_InstantRegret,          ATTACK_INDIVIDUAL, MOTHER,        DEBUFF_INDIVIDUAL, DAUGHTER} ;

    //A million stabs (2AP): Deal 8 damage to all enemies; Expose your daughter; Lose 10 HP.
    skillInfoMap[AMillionStabs] = {2, Skill::StateProtected::EXPOSED,      8, {},     10, {},                     NS_AMillionStabs, I_AMillionStabs, SPR_AMillionStabs,          ATTACK_ALL, MOTHER,        NONE, IRR} ;

    //Danse Macabre (3AP): Deal 12 damage to all enemies; Gain 2 Dodge; Apply 2 Vulnerable to all enemies; Lose 15 HP.
    skillInfoMap[DanseMacabre] = {3, Skill::StateProtected::NOCHANGES,      12, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE},     15, {Tag::Tags::DODGE, Tag::Tags::DODGE},                     NS_DanseMacabre, I_DanseMacabre, SPR_DanseMacabre,          ATTACK_ALL, MOTHER,        BUFF_INDIVIDUAL, MOTHER} ;

    //Hellfire* (2AP): Deal 3 damage; Apply 4 stacks of curse; Lose 5 HP
    skillInfoMap[Hellfire] = {2, Skill::StateProtected::NOCHANGES,      3, {Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE},     0, {},                     NS_Hellfire, I_Hellfire, SPR_Hellfire,          ATTACK_INDIVIDUAL, MOTHER,        NONE, IRR} ;


    //==================================LOCKED==================================
    skillInfoMap[LOCKED1] = {0, Skill::StateProtected::NOCHANGES,   0, {},     0, {},     NS_LOCKED, I_LOCKED, SPR_LOCKED,          NONE, IRR,        NONE, IRR};
    skillInfoMap[LOCKED2] = {0, Skill::StateProtected::NOCHANGES,   0, {},     0, {},     NS_LOCKED2, I_LOCKED2, SPR_LOCKED2,          NONE, IRR,        NONE, IRR};
    skillInfoMap[LOCKED3] = {0, Skill::StateProtected::NOCHANGES,   0, {},     0, {},     NS_LOCKED3, I_LOCKED3, SPR_LOCKED3,          NONE, IRR,        NONE, IRR};
    
    //==================================EMPTY==================================
    skillInfoMap[EMPTY] = {0, Skill::StateProtected::NOCHANGES,   0, {},     0, {},     NS_EMPTY, I_EMPTY, SPR_EMPTY,          NONE, IRR,        NONE, IRR};
    

    //==================================Enemies skill==================================
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
       