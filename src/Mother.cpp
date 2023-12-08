#include "Mother.h"
#include "GameData.h"
#include "Enemies.h"
#include "Camera.h"
#include "Game.h"
#include "Protected.h" 
#include "CombatState.h"
#include "NP.h"

int Mother::hp = GameData::hp; 
int Mother::damageDjinn = 0;
std::vector<Tag::Tags> Mother::tags = {};
bool Mother::activateRampage = false;
bool Mother::activateWeak = false;

std::weak_ptr<GameObject> Mother::motherInstance;

#ifdef DEBUG
  
#include <SDL2/SDL.h> 
#endif //DEBUG

// speed já está sendo inicializado pelo construtor de Vec2
Mother::Mother(GameObject &associated) : 
Component::Component(associated),
indicator(nullptr),
intention(nullptr),
lifeBarMother(nullptr), 
tagSpaceCount(0),
ScaleIntention(1),
ScaleIndicator(1){
 
}  
 
void Mother::Start()  
{  
    Sprite *mother_spr = new Sprite(associated, GetSpriteMother(), MOTHER_FC, MOTHER_FT/ MOTHER_FC);
    associated.AddComponent((std::shared_ptr<Sprite>)mother_spr); 
    associated.box.y -= associated.box.h;

    //===================================Hitbox==================================
    motherHitbox = Rect(associated.box.x, associated.box.y, 130, associated.box.h);

    associated.box.x -= (associated.box.w/MOTHER_FC - motherHitbox.w )/2;

    //==================================LifeBar====================================
    lifeBarMother = new LifeBar(associated, GameData::hpMax, hp, motherHitbox.w, motherHitbox.x); //width from hitbox
    associated.AddComponent(std::shared_ptr<LifeBar>(lifeBarMother));

    //If enemies starts with tags
    ApplyTags(tags); 

    lifeBarMother->SetCorruptedHP(GameData::hpCorrupted);
    //lifeBarMother->SetCurrentHP(hp);  

}

Mother::~Mother()
{
    for (int i = mothertags.size() - 1; i >= 0; i--) { //remove enemies tags
            mothertags.erase(mothertags.begin() + i);
    }

    Mother::tags.clear();

    DeleteIndicator();
    DeleteIntention();
}  
 
void Mother::Update(float dt) 
{       

    if(damageDjinn != 0){
        GameData::hpCorrupted += damageDjinn;
        int corruptedDamage = lifeBarMother->SetCorruptedHP(GameData::hpCorrupted);
        damageDjinn = 0;

        if(corruptedDamage > 0){
           hp = corruptedDamage; 
        }
        
    }
    std::cout << deathTransitionTime.Get() << std::endl;
    if (hp <= 0 ) {

        if(deathTransitionTime.Get() == 0){
            auto spriteComponent = associated.GetComponent("Sprite");
            auto spriteComponentPtr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
            if (spriteComponentPtr) {
                associated.RemoveComponent(spriteComponentPtr);
            }           
            Sprite* deadBody_spr = new Sprite(associated, GetSpriteMother(), MOTHER_FC, MOTHER_FT/ MOTHER_FC);
        
            deadBody_spr->SetFrame(0);

            associated.AddComponent(std::shared_ptr<Sprite>(deadBody_spr)); 

            CombatState::motherTransition = true;    
        }
        else if(deathTransitionTime.Get() >= MOTHER_DEATH_TIME * 0.9 && CombatState::motherTransition){
            //Increment np level
            GameData::npLevel++;
            
            //Update life
            float multiplerNP = 0;
            if(GameData::npLevel == 1){
                multiplerNP = 0.25f;
                Skill::AddSkill(Skill::SkillId::EMPTY, Skill::SkillId::LOCKED1);
            }
            else if(GameData::npLevel == 2){
                multiplerNP = 0.35f;
                Skill::AddSkill(Skill::SkillId::EMPTY, Skill::SkillId::LOCKED2);
            }

            //Update NP_UI
            NP::ChangeNPLevel();

            GameData::hpCorrupted = GameData::hpMax * multiplerNP;
            hp = GameData::hpMax - GameData::hpCorrupted;
            lifeBarMother->SetCurrentHP(hp);  
            lifeBarMother->SetCorruptedHP(GameData::hpCorrupted);

            associated.box.y += associated.box.h;

            //PROCESS new sprite
            auto spriteComponent = associated.GetComponent("Sprite");
            auto spriteComponentPtr = std::dynamic_pointer_cast<Sprite>(spriteComponent);
            if (spriteComponentPtr) {
                associated.RemoveComponent(spriteComponentPtr);
            }  
            Sprite* deadBody_spr = new Sprite(associated, GetSpriteMother(), MOTHER_FC, MOTHER_FT/ MOTHER_FC);
            deadBody_spr->SetFrame(0); 
            associated.AddComponent(std::shared_ptr<Sprite>(deadBody_spr)); 

           associated.box.y -= associated.box.h;

           CombatState::motherTransition = false;
        }
        
        deathTransitionTime.Update(dt);
    } 
    else{
        deathTransitionTime.Restart();
    }

    if(CombatState::InteractionSCreenActivate || CombatState::ChangingSides || CombatState::motherTransition){
        return;
    }

    IntentionAnimation(dt);
    IndicatorAnimation(dt);
 
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY()); 
 
    auto selectedSkill = Skill::selectedSkill;
    auto selectedSkillEnemy = Skill::selectedSkillEnemy;
    auto skillBack = Skill::skillBackToMother;

   

    //ENEMY TURN
    if(GameData::playerTurn == false){
        DeleteIntention();
        DeleteIndicator();

        //=============================Targeted skill sector=============================
        //Sector to manipulate interections involving mother being attacked
        if (selectedSkillEnemy){
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkillEnemy->GetId()];
            if((tempSkillInfo.attackType == Skill::AttackType::ATTACK_INDIVIDUAL || tempSkillInfo.attackType == Skill::AttackType::DEBUFF_INDIVIDUAL)
            && Skill::playerTargetType == Skill::MOTHER){

                ApplySkillToMother(tempSkillInfo.damage, tempSkillInfo.tags);
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
            if(tempSkillInfo.targetTypeAttacker == Skill::TargetType::MOTHER){
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
                if(tempSkillInfo.targetTypeAttacker != Skill::TargetType::MOTHER){ //With its mother who caste probably the buff is for the daughter
                    if (indicator == nullptr){
                        CreateIndicator(); // Create an indicator if it doesn't exist
                    }
                    else{
                        auto objComponent = indicator->GetComponent("Sprite");
                        auto objComponentPtr = std::dynamic_pointer_cast<Sprite>(objComponent);
                        if (motherHitbox.Contains(mousePos.x - Camera::pos.x * Game::resizer, mousePos.y- Camera::pos.y * Game::resizer)){
                            if (objComponentPtr) {
                                objComponentPtr->SetAlpha(255);                          
                            }
                             else{
                                if (objComponentPtr) {
                                    objComponentPtr->SetAlpha(INDICATOR_ALPHA);                          
                                }
                            }    
                        } 

                    } 
                    // Check if the mouse is over the enemy and left mouse button is pressed
                    //TODO case of being buff_all
                    if (motherHitbox.Contains(mousePos.x - Camera::pos.x * Game::resizer, mousePos.y- Camera::pos.y * Game::resizer) && inputManager.MousePress(LEFT_MOUSE_BUTTON)) {
                        AP::apCount -= tempSkillInfo.apCost;
                        ApplySkillToMother(tempSkillInfo.damage, tempSkillInfo.tags);
                        Mother::damageDjinn = tempSkillInfo.damageBack;
                        selectedSkill->SkillBack(tempSkillInfo.targetTypeBack);
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
            ApplySkillToMother(0, tempSkillInfo.tagsBack);
            skillBack->DeselectBack(tempSkillInfo.targetTypeBack);
        }
    }    
}

void Mother::SetupInteractionScreen(Skill::AttackType attackType, Skill::TargetType whoAttacks){
    CombatState::InteractionSCreenActivate = true;
    CombatState::attackType = attackType;
    CombatState::whoAttacks = whoAttacks;
    if(whoAttacks == Skill::TargetType::MOTHER){
        CombatState::whoReceives = Skill::TargetType::DAUGHTER;
    }
    else{
        CombatState::whoReceives = Skill::TargetType::MOTHER;
    }
    
}

void Mother::IndicatorAnimation(float dt) {
    if (indicator != nullptr) {
        auto objComponent = indicator->GetComponent("Sprite");
        auto objComponentPtr = std::dynamic_pointer_cast<Sprite>(objComponent);
        if (objComponentPtr) {
            auto scaleSprite = objComponentPtr->GetScale();

            objComponentPtr->SetAlpha(INDICATOR_ALPHA);

            // Set the target scale and animation speed
            float targetScale = INDICATOR_TIME_ANIMATION;
            float animationSpeed = INDICATOR_TIME_ANIMATION_V; 
 

            // Calculate the new scale based on time (dt)
            scaleSprite.x += ScaleIndicator * animationSpeed * dt;

            // Check if the scale has reached the minimum or maximum limit
            if (ScaleIndicator == 1 && scaleSprite.x >= targetScale) {
                // Set the scale to the target value and reverse the direction
                scaleSprite.x = targetScale;
                ScaleIndicator = -1;
            } else if (ScaleIndicator == -1 && scaleSprite.x <= 1.0f) {
                // Set the scale to the target value and reverse the direction
                scaleSprite.x = 1.0f;
                ScaleIndicator = 1;
            }

            // Center position original
            auto posXenterX = indicator->box.x + indicator->box.w / 2;
            auto posXenterY = indicator->box.y + indicator->box.h / 2;

            // Call SetScale with the correct number of arguments
            objComponentPtr->SetScale(scaleSprite.x, scaleSprite.x);

            // Postion correction
            indicator->box.x = posXenterX - indicator->box.w / 2;
            indicator->box.y = posXenterY - indicator->box.h / 2; 
        }
    }
}  

std::string Mother::GetSpriteMother() {
    if(GameData::npLevel == 0){
        return MOTHER_SPRITE;
    }
    else if(GameData::npLevel == 1){
        return MOTHER_SPRITE_NP1;
    }
    else if(GameData::npLevel == 2){ 
        return MOTHER_SPRITE_NP2;
    }
    else{
        return MOTHER_SPRITE_NP2;
    }

}

void Mother::CreateIndicator() {
    indicator = new GameObject(motherHitbox.x + motherHitbox.w/2, motherHitbox.y + motherHitbox.h + LIFEBAROFFSET);
    Sprite* indicator_spr = new Sprite(*indicator, MOTHER_INDICATOR_SPRITE);

    indicator->box.x -= indicator->box.w/2;
    indicator->box.y -= indicator->box.h;

    indicator->AddComponent(std::make_shared<Sprite>(*indicator_spr));
    Game::GetInstance().GetCurrentState().AddObject(indicator);
}

void Mother::DeleteIndicator() {
    if (indicator != nullptr) {
        indicator->RequestDelete();
        indicator = nullptr;
    }
}

void Mother::IntentionAnimation(float dt) {
    if (intention != nullptr) {
        auto objComponent = intention->GetComponent("Sprite");
        auto objComponentPtr = std::dynamic_pointer_cast<Sprite>(objComponent);
        if (objComponentPtr) {
            auto scaleSprite = objComponentPtr->GetScale();

            // Set the target scale and animation speed
            float targetScale = INTENTION_TIME_ANIMATION;
            float animationSpeed = INTENTION_TIME_ANIMATION_V;


            // Calculate the new scale based on time (dt)
            scaleSprite.x += ScaleIntention * animationSpeed * dt;

            // Check if the scale has reached the minimum or maximum limit
            if (ScaleIntention == 1 && scaleSprite.x >= targetScale) {
                // Set the scale to the target value and reverse the direction
                scaleSprite.x = targetScale;
                ScaleIntention = -1;
            } else if (ScaleIntention == -1 && scaleSprite.x <= 1.0f) {
                // Set the scale to the target value and reverse the direction
                scaleSprite.x = 1.0f;
                ScaleIntention = 1;
            }

            // Center position original
            auto posXenterX = intention->box.x + intention->box.w / 2;
            auto posXenterY = intention->box.y + intention->box.h / 2;

            // Call SetScale with the correct number of arguments
            objComponentPtr->SetScale(scaleSprite.x, scaleSprite.x);

            // Postion correction
            intention->box.x = posXenterX - intention->box.w / 2;
            intention->box.y = posXenterY - intention->box.h / 2;
        }
    }
} 

void Mother::CreateIntention() { 
    intention = new GameObject(motherHitbox.x+ motherHitbox.w/2, motherHitbox.y); 
    Sprite* intention_spr = new Sprite(*intention, MOTHER_INTENTON_SPRITE);
    intention->AddComponent(std::make_shared<Sprite>(*intention_spr));
    intention->box.x -= intention->box.w/2;
    intention->box.y -= intention->box.h/2;
    Game::GetInstance().GetCurrentState().AddObject(intention);
}
 
void Mother::DeleteIntention() {
    if (intention != nullptr) {
        intention->RequestDelete();
        intention = nullptr;
    }
}

void Mother::ApplySkillToMother(int damage, std::vector<Tag::Tags> tags) {
        float tagMultiplier = 1; //multiplier without tags
        bool dodge = false;
        if (HasTag(Tag::Tags::DODGE)){
            int dodgeChance = rand() % 2;
            if(dodgeChance == 1){
                ActivateTag(Tag::Tags::DODGE);
                damage = 0;
                dodge = true;
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
                tagMultiplier += 0.25;
            }
            if (Skill::HasTagRampageOrWeak.second){
                tagMultiplier -= 0.25;
            }

            
        }
        Skill::HasTagRampageOrWeak ={false, false}; //reset
        hp -= damage * tagMultiplier;
        
        ApplyTags(tags);
        

        if(damage > 0 || dodge){
            lifeBarMother->SetCurrentHP(hp);  // Update the enemy's HP bar
        }
 
}
 
void Mother::ApplyTags(std::vector<Tag::Tags> skillTags) {
    for (auto& tag : skillTags) {
        ActivateTag(tag);
        if (!(std::find(tags.begin(), tags.end(), tag) != tags.end())) {
            tags.push_back(tag);
            auto go_tag = AddObjTag(tag);
        }
        tagCountMap[tag]++;
        
        // Iterate over the list of weak_ptr to the tag GameObjects
            for (auto& weak_tag : mothertags) {
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
    }
}

void Mother::ActivateTag(Tag::Tags tag){
    for (auto& weak_tag : mothertags) {
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


std::weak_ptr<GameObject>  Mother::AddObjTag(Tag::Tags tag){ 
    std::weak_ptr<GameObject> weak_enemy = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);

    GameObject* tagObject = new GameObject();
    Tag* tag_behaviour = new Tag(*tagObject, tag, weak_enemy, tagCountMap[tag]);
    tagObject->AddComponent(std::shared_ptr<Tag>(tag_behaviour));

    tagObject->box.x = motherHitbox.x + TAGS_SPACING_X * tagSpaceCount;
    tagObject->box.y = motherHitbox.y + motherHitbox.h + TAGS_SPACING_Y;
    std::weak_ptr<GameObject> go_tag = Game::GetInstance().GetCurrentState().AddObject(tagObject);

    tagSpaceCount += 1; 
    mothertags.push_back(go_tag);  

    return go_tag; 
}

void Mother::RemoveOneTagAll() {
    std::vector<Tag::Tags> tagsToRemove;

    for (const auto& tag : tags) {
        if (tagCountMap.find(tag) != tagCountMap.end() && tagCountMap[tag] > 0) {
            if(tag == Tag::Tags::CURSE){
                hp -= tagCountMap[tag];
                lifeBarMother->SetCurrentHP(hp);
            }

            tagCountMap[tag]--;

            // Iterate over the list of weak_ptr to the tag GameObjects
            auto it = mothertags.begin();
            while (it != mothertags.end()) {
                auto tagGameObject = it->lock();
                if (tagGameObject) {
                    auto tagComponent = tagGameObject->GetComponent("Tag");
                    auto tagComponentPtr = std::dynamic_pointer_cast<Tag>(tagComponent);
                    if (tagComponentPtr && tagComponentPtr->GetTag() == tag) {
                        tagComponentPtr->UpdateQuantity(tagCountMap[tag]);
                        if (tagCountMap[tag] == 0) {
                            tagsToRemove.push_back(tag);
                            tagGameObject->RequestDelete();
                            it = mothertags.erase(it);
                        } else {
                            ++it;
                        }
                    } else {
                        ++it;
                    }
                } else {
                    it = mothertags.erase(it);
                }
            }
        }
    }

    // Remove the tags from the 'tags' list
    for (const auto& tag : tagsToRemove) {
        tags.erase(std::remove(tags.begin(), tags.end(), tag), tags.end());
    }

    // Re-create the tag UI
    RecreateTagUI();
}

void Mother::RecreateTagUI() {
    // Clear all existing tag objects
    for (auto& weak_tag : mothertags) {
        auto tagGameObject = weak_tag.lock();
        if (tagGameObject) {
            tagGameObject->RequestDelete();
        }
    }

    // Clear the list of tag objects
    mothertags.clear();

    tagSpaceCount = 0;

    // Recreate tag objects based on the current tag list
    for (const auto& tag : tags) {
        AddObjTag(tag);
    }
}


bool Mother::HasTag(Tag::Tags tagToCheck) {
    // Go through the enemy's tag list and check if the desired ta g is present. 
    for (const auto& tag : tags) {
        if (tag == tagToCheck) {  
            return true; // tag is present
        }
    }
    return false; // tag isnt present.
}



void Mother::Render()
{
#ifdef DEBUG
    Vec2 center(motherHitbox.GetCenter());
    SDL_Point points[5];

    Vec2 point = (Vec2(motherHitbox.x, motherHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = (Vec2(motherHitbox.x + motherHitbox.w, motherHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[1] = {(int)point.x, (int)point.y};

    point = (Vec2(motherHitbox.x + motherHitbox.w, motherHitbox.y + motherHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[2] = {(int)point.x, (int)point.y};

    point = (Vec2(motherHitbox.x, motherHitbox.y + motherHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

bool Mother::Is(std::string type){
    return (type == "Mother");
}

