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
#include "TagReader.h"

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
    if(skillSelected != nullptr){
        skillSelected->RequestDelete();
        skillSelected = nullptr;
    }
}

void Skill::Update(float dt) {

    //Creates border to sinalize selection of skill
    if(Skill::skillFromReward == this || Skill::skillToReward == this || Skill::selectedSkill == this){
        
        if(skillSelected == nullptr){
            skillSelected = new GameObject(associated.box.x + Camera::pos.x - 4, associated.box.y + Camera::pos.y -2);
            
            std::string haveJewel; 
            if(createJewel){
                haveJewel = SKILL_SELECTED_OBJ;
            }
            else{
                haveJewel = SKILL_SELECTED_OBJ_REWARD;
            }
            
            Sprite* skillSelected_behaviour = new Sprite(*skillSelected, haveJewel);
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

    if (associated.box.Contains(mousePos.x- Camera::pos.x * Game::resizer, mousePos.y- Camera::pos.y * Game::resizer)){ 

            if (true) {
                if (!readerSkill && skillClickTimer.Get() >= SKILL_CLICK_COOLDOWN) {
                    readerSkill = new GameObject(associated.box.x + associated.box.w/2 + Camera::pos.x, associated.box.y + associated.box.h/3 + Camera::pos.y);
                    Reader* readerSkill_behaviour = new Reader(*readerSkill, textSkill);
                    readerSkill->AddComponent(std::make_shared<Reader>(*readerSkill_behaviour));

                    readerSkill->box.x -= readerSkill->box.w/2;

                    //===============logic de tag reader =====================

                    bool isReversed = false;
                    if(readerSkill->box.x > RESOLUTION_WIDTH * Game::resizer * 0.6){
                        isReversed = true;
                    }


                    Skill::SkillInfo tempSkillInfo = skillInfoMap[id];

                    bool auxExposedFeature = false;
                    if(tempSkillInfo.isProtected != NOCHANGES){
                        auxExposedFeature = true;
                    }

                    std::vector<Tag::Tags> mergedTags;
                    mergedTags.reserve(tempSkillInfo.tags.size() + tempSkillInfo.tagsBack.size());
                    mergedTags.insert(mergedTags.end(), tempSkillInfo.tags.begin(), tempSkillInfo.tags.end());
                    mergedTags.insert(mergedTags.end(), tempSkillInfo.tagsBack.begin(), tempSkillInfo.tagsBack.end());

                    // Ordenando e removendo elementos duplicados
                    std::sort(mergedTags.begin(), mergedTags.end());
                    mergedTags.erase(std::unique(mergedTags.begin(), mergedTags.end()), mergedTags.end());


                    TagReader* tagReader_behaviour = new TagReader(*readerSkill,
                                                            auxExposedFeature,
                                                            mergedTags,
                                                            readerSkill->box,
                                                            isReversed);

                    readerSkill->AddComponent(std::make_shared<TagReader>(*tagReader_behaviour)); 

                    //===================================================================================

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
                               if(id < Skill::InstantRegret || id == Skill::EMPTY){
                                    skillToReward = this;
                               }      
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
        skillArray.push_back(Skill::RecklessSlash);
        skillArray.push_back(Skill::CautiousStrike);
        skillArray.push_back(Skill::MotherlyLove);
        skillArray.push_back(Skill::PocketSand);
        skillArray.push_back(Skill::NanaNanaNa);
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
    //Instant Regret (3AP): Deal 20 damage; Expose your daughter; Apply 1 Vulnerable to your daughter; Lose 4HP
    skillInfoMap[InstantRegret] = {3, Skill::StateProtected::EXPOSED,      20, {},     4, {Tag::Tags::VULNERABLE},                     NS_InstantRegret, I_InstantRegret, SPR_InstantRegret,          ATTACK_INDIVIDUAL, MOTHER,        DEBUFF_INDIVIDUAL, DAUGHTER} ;

    //A million stabs (2AP): Deal 8 damage to all enemies; Expose your daughter; Lose 5 HP.
    skillInfoMap[AMillionStabs] = {2, Skill::StateProtected::EXPOSED,      8, {},     5, {},                     NS_AMillionStabs, I_AMillionStabs, SPR_AMillionStabs,          ATTACK_ALL, MOTHER,        NONE, IRR} ;

    //Danse Macabre (3AP): Deal 12 damage to all enemies; Gain 2 Dodge; Apply 2 Vulnerable to all enemies; Lose 8 HP.
    skillInfoMap[DanseMacabre] = {3, Skill::StateProtected::NOCHANGES,      12, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE},     8, {Tag::Tags::DODGE, Tag::Tags::DODGE},                     NS_DanseMacabre, I_DanseMacabre, SPR_DanseMacabre,          ATTACK_ALL, MOTHER,        BUFF_INDIVIDUAL, MOTHER} ;

    //Hellfire* (2AP): Deal 3 damage; Apply 4 stacks of curse; Lose 3 HP
    skillInfoMap[Hellfire] = {2, Skill::StateProtected::NOCHANGES,      3, {Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE},     3, {},                     NS_Hellfire, I_Hellfire, SPR_Hellfire,          ATTACK_INDIVIDUAL, MOTHER,        NONE, IRR} ;

    //Gravedigger (3AP): Gain 3 Rampage; Apply 3 Vulnerable to all enemies; Lose 4 max HP.
    skillInfoMap[Gravedigger] = {3, Skill::StateProtected::NOCHANGES,      3, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE},     4, {Tag::Tags::RAMPAGE, Tag::Tags::RAMPAGE, Tag::Tags::RAMPAGE},                     NS_Gravedigger, I_Gravedigger, SPR_Gravedigger,          ATTACK_ALL, MOTHER,        BUFF_INDIVIDUAL, MOTHER} ;

    //Combustion (1AP): Deal 6 damage; Expose your daughter; Lose 2 max HP
    skillInfoMap[Combustion] = {1, Skill::StateProtected::EXPOSED,      6, {},     2, {},            NS_Combustion, I_Combustion, SPR_Combustion,          ATTACK_INDIVIDUAL, MOTHER,        NONE, IRR} ;

    //Devil’s Tears (1AP)*: Deal 1 damage to all enemies; Apply 4 curse; Lose 3 max HP.
    skillInfoMap[DevilsTears] = {1, Skill::StateProtected::NOCHANGES,      1, {Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE},     3, {},            NS_DevilsTears, I_DevilsTears, SPR_DevilsTears,          ATTACK_ALL, MOTHER,        NONE, IRR} ;

    //Final Sacrifice (3AP): Expose your daughter and apply 2 vulnerable to her; Deal 25 damage to all enemies; Lose 10 max HP.
    skillInfoMap[FinalSacrifice] = {3, Skill::StateProtected::EXPOSED,      25, {},     10, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE},            NS_FinalSacrifice, I_FinalSacrifice, SPR_FinalSacrifice,          ATTACK_ALL, MOTHER,        DEBUFF_INDIVIDUAL, DAUGHTER} ;

    //Tormented Soul* (3AP): Apply 12 curse; Lose 5 max HP.
    skillInfoMap[TormentedSoul] = {3, Skill::StateProtected::NOCHANGES,      0, {Tag::Tags::CURSE,Tag::Tags::CURSE,Tag::Tags::CURSE, Tag::Tags::CURSE,Tag::Tags::CURSE,Tag::Tags::CURSE, Tag::Tags::CURSE,Tag::Tags::CURSE,Tag::Tags::CURSE, Tag::Tags::CURSE,Tag::Tags::CURSE,Tag::Tags::CURSE},     5, {},            NS_TormentedSoul, I_TormentedSoul, SPR_TormentedSoul,          DEBUFF_INDIVIDUAL, MOTHER,        NONE, IRR} ;
    
    //Raining Blood (3AP): Deal 15 damage to all enemies; Gain 2 Rampage; Apply 2 Vulnerable; Lose 8 max HP.
    skillInfoMap[RainingBlood] = {3, Skill::StateProtected::NOCHANGES,      15, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE},     8, {Tag::Tags::RAMPAGE, Tag::Tags::RAMPAGE},            NS_RainingBlood, I_RainingBlood, SPR_RainingBlood,          ATTACK_ALL, MOTHER,        BUFF_INDIVIDUAL, MOTHER} ;

    //Master of Puppets (2AP): Deal 20 damage; Lose 7 max HP.
    skillInfoMap[MasterofPuppets] = {2, Skill::StateProtected::NOCHANGES,      20, {},     7, {},            NS_MasterofPuppets, I_MasterofPuppets, SPR_MasterofPuppets,          ATTACK_INDIVIDUAL, MOTHER,        NONE, IRR} ;

    //Iron Maiden (1AP): Protect your daughter, apply 5 vulnerable to her; Apply 3 Vulnerable and 3 Weak to one enemy; Lose 3 max HP
    skillInfoMap[IronMaiden] = {1, Skill::StateProtected::PROTECTED,      0, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::WEAK, Tag::Tags::WEAK, Tag::Tags::WEAK},     3, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE},            NS_IronMaiden, I_IronMaiden, SPR_IronMaiden,          DEBUFF_INDIVIDUAL, MOTHER,        DEBUFF_INDIVIDUAL, DAUGHTER} ;

    //Deafening Whisper (2AP): Deal 10 damage; Apply 2 Weak to target ; Lose 3 max HP
    skillInfoMap[DeafeningWhisper] = {2, Skill::StateProtected::NOCHANGES,      10, {Tag::Tags::WEAK, Tag::Tags::WEAK},     3, {},            NS_DeafeningWhisper, I_DeafeningWhisper, SPR_DeafeningWhisper,          ATTACK_INDIVIDUAL, MOTHER,         NONE, IRR} ;

    //Into the Void (3AP): Apply 5 Vulnerable to you and all enemies; Gain 2 Rampage; Lose 3 max HP.
    skillInfoMap[IntotheVoid] = {3, Skill::StateProtected::NOCHANGES,      0, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE},     3, {Tag::Tags::RAMPAGE, Tag::Tags::RAMPAGE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE},            NS_IntotheVoid, I_IntotheVoid, SPR_IntotheVoid,          DEBUFF_ALL, MOTHER,          DEBUFF_INDIVIDUAL, MOTHER} ;

    //Fear of the Dark (1AP): Apply 2 dodge to all enemies; Apply 3 weak and 3 vulnerable to all enemies. Lose 3 max HP
    skillInfoMap[FearoftheDark] = {1, Skill::StateProtected::NOCHANGES,      0, {Tag::Tags::DODGE, Tag::Tags::DODGE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE, Tag::Tags::WEAK, Tag::Tags::WEAK, Tag::Tags::WEAK},     3, {},            NS_FearoftheDark, I_FearoftheDark, SPR_FearoftheDark,          DEBUFF_ALL, MOTHER,          NONE, IRR} ;
 
    //Elimination* (3AP): Deal 25 damage; Gain 15 Curse; Lose 2 max HP 
    skillInfoMap[Elimination] = {3, Skill::StateProtected::NOCHANGES,      25, {},     
                                    2, {Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE, Tag::Tags::CURSE},
                                    NS_Elimination, I_Elimination, SPR_Elimination,          ATTACK_INDIVIDUAL, MOTHER,          DEBUFF_INDIVIDUAL, MOTHER} ;


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



    // Cut: Deal 6 damage
    skillInfoMap[E_Cut] = {0, Skill::StateProtected::NOCHANGES,   6, {},     0, {},     NS_Generic, I_Generic, SPR_Generic,          ATTACK_INDIVIDUAL, IRR,        NONE, IRR} ;

    //Whip: Deal 3 damage; Apply 2 Frail to target.
    skillInfoMap[E_Whip] = {0, Skill::StateProtected::NOCHANGES,   3, {Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE},     0, {},     NS_Generic, I_Generic, SPR_Generic,          ATTACK_INDIVIDUAL, IRR,        NONE, IRR} ;
  
    //Evil Chant: Apply 2 weak to target
    skillInfoMap[E_Evil_Chant] = {0, Skill::StateProtected::NOCHANGES,   0, {Tag::Tags::WEAK, Tag::Tags::WEAK},     0, {},     NS_Generic, I_Generic, SPR_Generic,          DEBUFF_INDIVIDUAL, IRR,        NONE, IRR} ;


    // Shiv: Deal 9 damage 
    skillInfoMap[E_Shiv] = {0, Skill::StateProtected::NOCHANGES,   9, {},     0, {},     NS_Generic, I_Generic, SPR_Generic,          ATTACK_INDIVIDUAL, IRR,        NONE, IRR} ;

    //Tentacle Strike: Deal 12 damage
    skillInfoMap[E_Tentacle_Strike] = {0, Skill::StateProtected::NOCHANGES,   12, {},     0, {},     NS_Generic, I_Generic, SPR_Generic,          ATTACK_INDIVIDUAL, IRR,       NONE, IRR} ;

    //Bite: Deal 9 damage 
    skillInfoMap[E_Bite] = {0, Skill::StateProtected::NOCHANGES,   9, {},     0, {},     NS_Generic, I_Generic, SPR_Generic,          ATTACK_INDIVIDUAL, IRR,        NONE, IRR} ;


    //Beak: Deal 3 damage
    skillInfoMap[E_Beak] = {0, Skill::StateProtected::NOCHANGES,   3, {},     0, {},     NS_Generic, I_Generic, SPR_Generic,          ATTACK_INDIVIDUAL, IRR,        NONE, IRR} ;

    //Unnerving Presence: Apply 2 Weak and 2 Vulnerable to target
    skillInfoMap[E_Unnerving_Presence] = {0, Skill::StateProtected::NOCHANGES,   0, {Tag::Tags::WEAK, Tag::Tags::WEAK, Tag::Tags::VULNERABLE, Tag::Tags::VULNERABLE,},     0, {},     NS_Generic, I_Generic, SPR_Generic,          DEBUFF_INDIVIDUAL, IRR,       NONE, IRR} ;

    //Guttural Scream: Apply 2 rampage to all allies
    skillInfoMap[E_Guttural_Scream] = {0, Skill::StateProtected::NOCHANGES,   0, {Tag::Tags::RAMPAGE, Tag::Tags::RAMPAGE},     0, {},     NS_Generic, I_Generic, SPR_Generic,          BUFF_ALL, IRR,       NONE, IRR} ;

} 
       