#include "Daughter.h"
#include "GameData.h"
#include "Enemies.h"
#include "Camera.h"
#include "Game.h"
#include "Protected.h" 
#include "CombatState.h"




#ifdef DEBUG

#include <SDL2/SDL.h>
#endif //DEBUG


int Daughter::hp = DAUGHTER_HP; 
std::vector<Tag::Tags> Daughter::tags = {};
bool Daughter::activateRampage = false;
bool Daughter::activateWeak = false;

Daughter::Daughter(GameObject &associated) : 
Component::Component(associated),
indicator(nullptr),
intention(nullptr),
lifeBarDaughter(nullptr),
tagSpaceCount(0){

}

void Daughter::Start() 
{
    Sprite *daughter_spr = new Sprite(associated, DAUGHTER_SPRITE);
    associated.AddComponent((std::shared_ptr<Sprite>)daughter_spr); 
    associated.box.y -= associated.box.h;

    //===================================Hitbox==================================
    daughterHitbox = Rect(associated.box.x, associated.box.y, 130, associated.box.h);

    associated.box.x -= (associated.box.w - daughterHitbox.w )/2;

    //==================================LifeBar====================================
    lifeBarDaughter = new LifeBar(associated, hp, hp, daughterHitbox.w, daughterHitbox.x); //width from hitbox
    associated.AddComponent(std::shared_ptr<LifeBar>(lifeBarDaughter));

    //If enemies starts with tags
    ApplyTags(tags);

    lifeBarDaughter->SetCurrentHP(hp); 

}
 
Daughter::~Daughter()
{
    for (int i = daughtertags.size() - 1; i >= 0; i--) { //remove enemies tags
            daughtertags.erase(daughtertags.begin() + i);
    }
    
}

void Daughter::Update(float dt)
{   
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());

    auto selectedSkill = Skill::selectedSkill;
    auto selectedSkillEnemy = Skill::selectedSkillEnemy;
    auto skillBack = Skill::skillBackToDaughter;

    // Check if the enemy's HP is zero or below and request deletion
    //if (hp <= 0) {
    //    DeleteEnemyIndicator();
    //    associated.RequestDelete();
    //    return; // Early exit if the enemy is no longer alive
    //} 


    


    //ENEMY TURN
    if(GameData::playerTurn == false){
        DeleteIntention();
        DeleteIndicator();

        //=============================Targeted skill sector=============================
        //Sector to manipulate interections involving daughter being attacked
        if (Skill::selectedSkillEnemy){
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkillEnemy->GetId()];
            if((tempSkillInfo.attackType == Skill::AttackType::ATTACK_INDIVIDUAL || tempSkillInfo.attackType == Skill::AttackType::DEBUFF_INDIVIDUAL)
            && Skill::playerTargetType == Skill::DAUGHTER){

                ApplySkillToDaughter(tempSkillInfo.damage, tempSkillInfo.tags);
                Skill::selectedSkillEnemy = nullptr;
                Enemies::enemyAttacking = false;
                SetupInteractionScreen(tempSkillInfo.attackType, tempSkillInfo.targetTypeAttacker);
            }
        }

    
    }

    //PLAYER TURN
    else{
        
        if(activateRampage){
            ActivateTag(Tag::Tags::RAMPAGE);
            activateRampage = false;
        }
        if(activateWeak){
            ActivateTag(Tag::Tags::WEAK);
            activateWeak = false;
        }

        //--------------Intention manager------------------
        //Shows who wants to attack
        if (selectedSkill) {
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];
            if(tempSkillInfo.targetTypeAttacker == Skill::TargetType::DAUGHTER){
                if(intention == nullptr){
                    CreateIntention();
                }                      
            }else{
                DeleteIntention();
            }
        
        }else{
            DeleteIntention(); 
        }




        //=============================Skill buff sector==============================
         if (selectedSkill) {// Check if a skill is selected
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];
            if(tempSkillInfo.attackType == Skill::AttackType::BUFF_INDIVIDUAL || tempSkillInfo.attackType == Skill::AttackType::BUFF_ALL ){
                if(tempSkillInfo.targetTypeAttacker != Skill::TargetType::DAUGHTER){ //With its mother who caste probably the buff is for the daughter
                    if (indicator == nullptr){
                        CreateIndicator(); // Create an indicator if it doesn't exist
                    }

                    // Check if the mouse is over the enemy and left mouse button is pressed
                    //TODO case of being buff_all
                    if (daughterHitbox.Contains(mousePos.x, mousePos.y) && inputManager.MousePress(LEFT_MOUSE_BUTTON)) {
                        AP::apCount -= tempSkillInfo.apCost;
                        ApplySkillToDaughter(tempSkillInfo.damage, tempSkillInfo.tags);
                        selectedSkill->Deselect();

                        if(tempSkillInfo.isProtected == Skill::StateProtected::PROTECTED){
                            Protected::isProtected = true;
                        }
                        else if(tempSkillInfo.isProtected == Skill::StateProtected::EXPOSED){
                            Protected::isProtected = false;
                        }

                        SetupInteractionScreen(tempSkillInfo.attackType, tempSkillInfo.targetTypeAttacker);

                    } 
                }
                else{
                    DeleteIndicator();
                }
            }
            else{
                DeleteIndicator();// Delete the  indicator if it exists skill type not buff
            }                               
        }else {
            DeleteIndicator();// Delete the  indicator if it exists
        }    

        //=============================Skill back sector=================================
        //Sector to manipulate interections involving enemies being attacked

        if (skillBack != nullptr) {
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[skillBack->GetId()];
            ApplySkillToDaughter(tempSkillInfo.damageBack, tempSkillInfo.tagsBack);
            skillBack->DeselectBack(tempSkillInfo.targetTypeBack);
        }
    }    
}


void Daughter::SetupInteractionScreen(Skill::AttackType attackType, Skill::TargetType whoAttacks){
    CombatState::InteractionSCreenActivate = true;
    CombatState::attackType = attackType;
    CombatState::whoAttacks = whoAttacks;
    if(whoAttacks == Skill::TargetType::DAUGHTER){
        CombatState::whoReceives = Skill::TargetType::MOTHER;
    }
    else{
        CombatState::whoReceives = Skill::TargetType::DAUGHTER;
    }
}

void Daughter::CreateIndicator() {
    indicator = new GameObject(daughterHitbox.x, daughterHitbox.y + daughterHitbox.h);
    Sprite* indicator_spr = new Sprite(*indicator, DAUGHTER_INDICATOR_SPRITE);

    // Scale the enemy indicator
    float percentageEnemyWidth = daughterHitbox.w / indicator->box.w;
    indicator_spr->SetScale(percentageEnemyWidth, 1);
    indicator->AddComponent(std::make_shared<Sprite>(*indicator_spr));
    Game::GetInstance().GetCurrentState().AddObject(indicator);
}


void Daughter::DeleteIndicator() {
    if (indicator != nullptr) {
        indicator->RequestDelete();
        indicator = nullptr;
    }
}

void Daughter::CreateIntention() {
    intention = new GameObject(daughterHitbox.x+ daughterHitbox.w/2, daughterHitbox.y);
    Sprite* intention_spr = new Sprite(*intention, DAUGHTER_INTENTON_SPRITE);
    intention->AddComponent(std::make_shared<Sprite>(*intention_spr));
    intention->box.x -= intention->box.w/2;
    intention->box.y -= intention->box.h/2;
    Game::GetInstance().GetCurrentState().AddObject(intention);
}

void Daughter::DeleteIntention() {
    if (intention != nullptr) {
        intention->RequestDelete();
        intention = nullptr;
    }
}

void Daughter::ApplySkillToDaughter(int damage, std::vector<Tag::Tags> tags) {
        float tagMultiplier = 1; //multiplier without tags

        if (HasTag(Tag::Tags::DODGE)){
            int dodgeChance = rand() % 2;
            if(dodgeChance == 1){
                ActivateTag(Tag::Tags::DODGE);
                damage = 0;
            }
        }

        if(damage > 0){
            if (HasTag(Tag::Tags::RESILIENCE)){
                ActivateTag(Tag::Tags::RESILIENCE);
                tagMultiplier -= 0.5; 
            }
            if (HasTag(Tag::Tags::VULNERABLE)){
                ActivateTag(Tag::Tags::VULNERABLE);
                tagMultiplier += 0.5; 
            }

            //============RAMPAGE and WEAK sector=================//
            if (Skill::HasTagRampageOrWeak.first){
                tagMultiplier += 0.5;
            }
            if (Skill::HasTagRampageOrWeak.second){
                tagMultiplier -= 0.5;
            }

        }
        Skill::HasTagRampageOrWeak ={false, false}; //reset
        hp -= damage * tagMultiplier;
        ApplyTags(tags);
        lifeBarDaughter->SetCurrentHP(hp);  // Update the enemy's HP bar
 
}
 
void Daughter::ApplyTags(std::vector<Tag::Tags> skillTags) {
    for (auto& tag : skillTags) {
        ActivateTag(tag);
        if (tagCountMap.find(tag) != tagCountMap.end()) {
            // The tag already exists, increment the counter
            tagCountMap[tag]++;

            // Iterate over the list of weak_ptr to the tag GameObjects
            for (auto& weak_tag : daughtertags) {
                auto tagGameObject = weak_tag.lock();  // Get the GameObject
                if (tagGameObject) {
                    // Try to retrieve the "Tag" component
                    auto tagComponent = tagGameObject->GetComponent("Tag");
                    auto tagComponentPtr = std::dynamic_pointer_cast<Tag>(tagComponent);
                    if (tagComponentPtr) {
                        // Now, you can access the "tag" property of the "Tag" component
                        if (tagComponentPtr->GetTag() == tag) {
                            tagComponentPtr->UpdateQuantity(tagCountMap[tag]);
                        }
                    }
                }
            }
        } else {
            // The tag doesn't exist in the map, add it with a counter of 1
            tagCountMap[tag] = 1;
            tags.push_back(tag);
            auto go_tag = AddObjTag(tag);
            

        }
    }
}

void Daughter::ActivateTag(Tag::Tags tag){
    for (auto& weak_tag : daughtertags) {
        auto tagGameObject = weak_tag.lock();  // Get the GameObject
        if (tagGameObject) {
            // Try to retrieve the "Tag" component 
            auto tagComponent = tagGameObject->GetComponent("Tag");
            auto tagComponentPtr = std::dynamic_pointer_cast<Tag>(tagComponent);
            if (tagComponentPtr) {
                // Now, you can access the "tag" property of the "Tag" component
                if (tagComponentPtr->GetTag() == tag) {
                    tagComponentPtr->AcivateTag(tagComponentPtr->GetTagSprite(tag));
                }
            }
        }
    }
}


std::weak_ptr<GameObject>  Daughter::AddObjTag(Tag::Tags tag){ 
    std::weak_ptr<GameObject> weak_enemy = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);

    GameObject* tagObject = new GameObject();
    Tag* tag_behaviour = new Tag(*tagObject, tag, weak_enemy, 1);
    tagObject->AddComponent(std::shared_ptr<Tag>(tag_behaviour));

    tagObject->box.x = daughterHitbox.x + TAGS_SPACING_X * tagSpaceCount;
    tagObject->box.y = daughterHitbox.y + daughterHitbox.h + TAGS_SPACING_Y;
    std::weak_ptr<GameObject> go_tag = Game::GetInstance().GetCurrentState().AddObject(tagObject);

    tagSpaceCount += 1;
    daughtertags.push_back(go_tag);  

    return go_tag;
}

bool Daughter::HasTag(Tag::Tags tagToCheck) {
    // Go through the enemy's tag list and check if the desired tag is present. 
    for (const auto& tag : tags) {
        if (tag == tagToCheck) {
            return true; // tag is present
        }
    }
    return false; // tag isnt present.
}



void Daughter::Render()
{
#ifdef DEBUG
    Vec2 center(daughterHitbox.GetCenter());
    SDL_Point points[5];

    Vec2 point = (Vec2(daughterHitbox.x, daughterHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = (Vec2(daughterHitbox.x + daughterHitbox.w, daughterHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[1] = {(int)point.x, (int)point.y};

    point = (Vec2(daughterHitbox.x + daughterHitbox.w, daughterHitbox.y + daughterHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[2] = {(int)point.x, (int)point.y};

    point = (Vec2(daughterHitbox.x, daughterHitbox.y + daughterHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

bool Daughter::Is(std::string type){
    return (type == "Daughter");
}

