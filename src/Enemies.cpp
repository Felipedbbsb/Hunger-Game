#include "Enemies.h"
#include "InputManager.h"
#include "Skill.h" // Include the Skill header to access selectedSkill and SkillInfo
#include "LifeBar.h"
#include "Tag.h"
#include "Game.h"
#include "AP.h" 
#include "Protected.h" 
#include "GameData.h" 
#include "Mother.h"
#include "Daughter.h"  
#include "CombatState.h"
#include "Camera.h"
#include <algorithm> 

#ifdef DEBUG
#include "Game.h"

#include <SDL2/SDL.h> 
#endif //DEBUG

// Initialize the static enemyInfoMap
std::map<Enemies::EnemyId, Enemies::EnemyInfo> Enemies::enemyInfoMap;

std::vector<std::weak_ptr<GameObject>> Enemies::enemiesArray;

int Enemies::enemiesCount = 0; 

int Enemies::SkillAllenemies = 0;//how many left enemies to receive skill effects

int Enemies::provokedEnemies = 0;//how many left enemies has provoke

int Enemies::enemiesToAttack = 0;//how many left enemies to attack

bool Enemies::enemyAttacking = false;
 
Enemies::Enemies(GameObject& associated, EnemyId id)
    : Component::Component(associated), 
    enemyIndicator(nullptr), 
    intention(nullptr),
    id(id), 
    lifeBarEnemy(nullptr), 
    tagSpaceCount(0),
    thisEnemyAttacked(false),
    intentionTimer(),
    ScaleIntention(1),
    ScaleIndicator(1),
    selectSFX(nullptr) 
    {  
        EnemyInfo& enemyInfo = enemyInfoMap[id];
        hp = enemyInfo.hp.first + rand() % (enemyInfo.hp.second - enemyInfo.hp.first + 1);
        tags = enemyInfo.tags;
        name = enemyInfo.name;
        iconPath = enemyInfo.iconPath;
        skills = enemyInfo.skills; 
 
    
        enemiesCount += 1;    //one more enemy 
}
 
void Enemies::Start() {
    new Sprite(associated, iconPath, ENEMY_FC, ENEMY_FT/ENEMY_FC); 
  
    associated.box.y -= associated.box.h; 

    //===================================Hitbox==================================
    enemyHitbox = Rect(associated.box.x, associated.box.y, 130, associated.box.h);

    associated.box.x -= (associated.box.w/ENEMY_FC - enemyHitbox.w )/2;

    //==================================LifeBar====================================
    lifeBarEnemy = new LifeBar(associated, hp, hp, enemyHitbox.w, enemyHitbox.x); //width from hitbox


    //If enemies starts with tags
    ApplyTags(tags);

    //lifeBarEnemy->SetCurrentHP(hp);

    if(enemiesToAttack == 0){//init enemies attack turn
        enemiesToAttack = enemiesCount;
    } 

}

Enemies::~Enemies() { 

    for (int i = enemytags.size() - 1; i >= 0; i--) { //remove enemies tags
            enemytags.erase(enemytags.begin() + i);
    }

    if(HasTag(Tag::Tags::PROVOKE)){
        provokedEnemies -= 1;
    }

    DeleteEnemyIndicator();
    DeleteIntention();

    delete intention;
    delete enemyIndicator;
    
    enemiesCount -= 1;
    enemiesToAttack-= 1;

     // Remove the enemy from the enemiesArray vector
    enemiesArray.erase(std::remove_if(enemiesArray.begin(), enemiesArray.end(),
        [this](const std::weak_ptr<GameObject>& enemy) {
            return enemy.lock().get() == &associated;
        }), enemiesArray.end());

    if(selectSFX != nullptr){
        selectSFX->RequestDelete();
        selectSFX = nullptr;
        
    }

    delete selectSFX ;
}

void Enemies::Update(float dt) {
// Check if the enemy's HP is zero or below and request deletion
    if (hp <= 0) {
        GameObject *deadBody  = new GameObject(associated.box.x, associated.box.y);
        Sprite *deadBody_spr = new Sprite(*deadBody, iconPath, ENEMY_FC, ENEMY_FT/ ENEMY_FC, 1.5); 
        deadBody_spr->SetFrame(0); 
        Game::GetInstance().GetCurrentState().AddObject(deadBody);


        associated.RequestDelete();
        return; // Early exit if the enemy is no longer alive

    } 
    //=============================//=============================//=============================//=============================



    if(CombatState::InteractionSCreenActivate || CombatState::ChangingSides || CombatState::motherTransition){
        return;
    }


    IntentionAnimation(dt);
    IndicatorAnimation(dt);
    

    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());

    auto selectedSkill = Skill::selectedSkill;
    

//=============================//=============================//=============================//=============================
    //Iterator for all skill types, counts number of left enemies to receive skill from player
    if(SkillAllenemies > 0){
        Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];
        if(GameData::playerTurn == true){
            if (!provokedEnemies ||  (provokedEnemies != 0  && HasTag(Tag::Tags::PROVOKE))){
                ApplySkillToSingleEnemy(tempSkillInfo.damage, tempSkillInfo.tags);
                CombatState::enemiesArrayIS.push_back(id);
            }
        
            SkillAllenemies -= 1; //less one enemy to receive skill
            
            if(SkillAllenemies == 0){ //no more enemies
                AP::apCount -= tempSkillInfo.apCost;
                selectedSkill->SkillBack(tempSkillInfo.targetTypeBack);
                selectedSkill->Deselect();
                enemyAttacking = false;
                SetupInteractionScreen(tempSkillInfo.attackType, tempSkillInfo.targetTypeAttacker);

            }
        }
        //ENEMY TURN
        else{
            ApplySkillToSingleEnemy(tempSkillInfo.damage, tempSkillInfo.tags);
            SkillAllenemies -= 1; //less one enemy to receive skill
            CombatState::enemiesArrayIS.push_back(id);
            if(SkillAllenemies == 0){ //no more enemies
                selectedSkill->Deselect();
                enemyAttacking = false;
                DeleteIntention();
                SetupInteractionScreen(tempSkillInfo.attackType, tempSkillInfo.targetTypeAttacker);

            }
        }

    }

                    




//=============================//=============================//=============================//=============================
    //PLAYER TURN
    if(GameData::playerTurn == true){

        thisEnemyAttacked = false;

        //=============================Targeted skill sector=============================
        //Sector to manipulate interections involving enemies being attacked
        if (selectedSkill) {// Check if a skill is selected

            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];
            if(tempSkillInfo.attackType == Skill::AttackType::ATTACK_INDIVIDUAL || tempSkillInfo.attackType == Skill::AttackType::ATTACK_ALL || 
            tempSkillInfo.attackType == Skill::AttackType::DEBUFF_INDIVIDUAL || tempSkillInfo.attackType == Skill::AttackType::DEBUFF_ALL){
                if (enemyIndicator == nullptr && (!provokedEnemies ||  (provokedEnemies != 0  && HasTag(Tag::Tags::PROVOKE)))) {
                    CreateEnemyIndicator();// Create an enemy indicator if it doesn't exist    
                }       // and if any enemie has provoke

                if(enemyIndicator != nullptr){
                    auto objComponent = enemyIndicator->GetComponent("Sprite");
                    auto objComponentPtr = std::dynamic_pointer_cast<Sprite>(objComponent);
                    if (enemyHitbox.Contains(mousePos.x - Camera::pos.x * Game::resizer, mousePos.y- Camera::pos.y * Game::resizer)){
                        if (objComponentPtr) {
                            objComponentPtr->SetAlpha(255);  
                            if(selectSFX == nullptr){
                                selectSFX = new GameObject();
                                Sound *selectSFX_sound = new Sound(*selectSFX, SKILL_SELECTION); 
                                selectSFX_sound->Play(1);
                            }     
                        }
                        else{
                            if (objComponentPtr) {
                                objComponentPtr->SetAlpha(INDICATOR_ALPHA);                          
                            }
                        }    
                    }
                    else{
                        if(selectSFX != nullptr){
                            selectSFX->RequestDelete();
                            selectSFX = nullptr;
                        }
                    }

                }
 
                // Check if the mouse is over the enemy and left mouse button is pressed
                if (enemyHitbox.Contains(mousePos.x - Camera::pos.x * Game::resizer, mousePos.y - Camera::pos.y * Game::resizer) && inputManager.MousePress(LEFT_MOUSE_BUTTON)) {
                    if (!provokedEnemies ||  (provokedEnemies && HasTag(Tag::Tags::PROVOKE))){
                        //checks if any enemie has provoke
                        ApplySkillToEnemy();
                        GameObject* selectedSFX = new GameObject();
                        Sound *selectSFX_sound = new Sound(*selectedSFX, SKILL_SELECTION_CONFIRMED); 

                        selectSFX_sound->Play(1);

                    }
                } 
            } 
            else{
                DeleteEnemyIndicator();// Delete the enemy indicator if it exists skill type not attack
            }                               
        }else {
            DeleteEnemyIndicator();// Delete the enemy indicator if it exists
        }

    }

    //ENEMY TURN
    else{
        
        DeleteEnemyIndicator();
        //=============================Targeted skill sector=============================
        //Sector to manipulate interections involving mother being attacked
        if(!enemyAttacking && !thisEnemyAttacked && Skill::selectedSkill == nullptr){
    
            // Check if the enemy has at least two skills
            if (skills.size() >= 2) {
                int randomSkillIndex = rand() % 2; //chooses between first or second skill
                Skill::SkillId selectedSkillId = skills[randomSkillIndex];
                
                // Moves the selected skill to the end of the vector
                std::swap(skills[randomSkillIndex], skills.back());

                GameObject* generic = new GameObject();
                // Now, the selected skill is in the last position of the vector
                Skill::selectedSkill = new Skill(*generic, selectedSkillId, nullptr, false);
            }


            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[Skill::selectedSkill->GetId()];
            // Randomly decides player target if its a attack to them
            if(tempSkillInfo.attackType == Skill::AttackType::DEBUFF_INDIVIDUAL ||
            tempSkillInfo.attackType == Skill::AttackType::ATTACK_INDIVIDUAL){
                
                if(Protected::isProtected == false){
                    Skill::TargetType validTargets[] = {Skill::TargetType::MOTHER, Skill::TargetType::DAUGHTER};
                    int randomIndex  = rand() % 2;
                    Skill::playerTargetType = validTargets[randomIndex];
                }
                else{
                    Skill::playerTargetType = Skill::TargetType::MOTHER;
                }
            }


            enemyAttacking = true;
            thisEnemyAttacked = true;
            enemiesToAttack -= 1; 

            intentionTimer.Restart(); //time to "select" target
            CreateIntention();

        }

        //Time to remove skill from buff to properly activate it
        if(selectedSkill != nullptr && intention != nullptr){
            intentionTimer.Update(dt);
            if(intentionTimer.Get() >= INTENTION_COOLDOWN){
                Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[Skill::selectedSkill->GetId()];
                if(tempSkillInfo.attackType == Skill::AttackType::ATTACK_INDIVIDUAL || tempSkillInfo.attackType == Skill::AttackType::ATTACK_ALL ||
                tempSkillInfo.attackType == Skill::AttackType::DEBUFF_INDIVIDUAL || tempSkillInfo.attackType == Skill::AttackType::DEBUFF_ALL){
                    Skill::selectedSkillEnemy = Skill::selectedSkill;
                    Skill::selectedSkill = nullptr;
                    
                    //============RAMPAGE and WEAK sector=================//
                    if(tempSkillInfo.attackType == Skill::AttackType::ATTACK_INDIVIDUAL || tempSkillInfo.attackType == Skill::AttackType::ATTACK_ALL){
                        bool hasRampage = false;
                        bool hasWeak = false;
                        if (HasTag(Tag::Tags::RAMPAGE)){
                            ActivateTag(Tag::Tags::RAMPAGE);
                            hasRampage = true;
                        }
                        if (HasTag(Tag::Tags::WEAK)){
                            ActivateTag(Tag::Tags::WEAK);
                            hasWeak = true;
                        } 
                        Skill::HasTagRampageOrWeak ={hasRampage, hasWeak};
                    }    
                    CombatState::enemiesArrayIS.push_back(id);
                }

                //=============================Skill buff sector==============================
                else if(tempSkillInfo.attackType == Skill::AttackType::BUFF_INDIVIDUAL){
                    ApplySkillToSingleEnemy(tempSkillInfo.damage, tempSkillInfo.tags); 
                    Skill::selectedSkill = nullptr;
                    enemyAttacking = false;
                    DeleteIntention();
                    CombatState::enemiesArrayIS.push_back(id);
                    SetupInteractionScreen(tempSkillInfo.attackType, tempSkillInfo.targetTypeAttacker);

                }
                else if(tempSkillInfo.attackType == Skill::AttackType::BUFF_ALL){
                    ApplySkillToAllEnemies();
                }   
                intentionTimer.Restart(); 

            }

            
            
        }

        //=============================Skill back sector=================================
        //Sector to manipulate interections involving enemies being attacked
        if(Skill::selectedSkillEnemy != nullptr && intention != nullptr){
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[Skill::selectedSkillEnemy->GetId()];
            if(tempSkillInfo.attackTypeBack == Skill::AttackType::BUFF_INDIVIDUAL
            || tempSkillInfo.attackTypeBack == Skill::AttackType::BUFF_ALL){
                ApplySkillToSingleEnemy(tempSkillInfo.damageBack, tempSkillInfo.tagsBack);     
            }
        }  



        //Finished deciding skill ready to attack
        if(Skill::selectedSkillEnemy != nullptr){
            DeleteIntention();
        }
        
    }
        

    


}


void Enemies::SetupInteractionScreen(Skill::AttackType attackType, Skill::TargetType whoAttacks){
    CombatState::InteractionSCreenActivate = true;
    CombatState::attackType = attackType;
    CombatState::whoAttacks = whoAttacks;
}

void Enemies::IndicatorAnimation(float dt) {
    if (enemyIndicator != nullptr) {
        auto objComponent = enemyIndicator->GetComponent("Sprite");
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
            auto posXenterX = enemyIndicator->box.x + enemyIndicator->box.w / 2;
            auto posXenterY = enemyIndicator->box.y + enemyIndicator->box.h / 2;

            // Call SetScale with the correct number of arguments
            objComponentPtr->SetScale(scaleSprite.x, scaleSprite.x);

            // Postion correction
            enemyIndicator->box.x = posXenterX - enemyIndicator->box.w / 2;
            enemyIndicator->box.y = posXenterY - enemyIndicator->box.h / 2;
        }
    }
} 
 
void Enemies::CreateEnemyIndicator() {
    enemyIndicator = new GameObject(enemyHitbox.x + enemyHitbox.w/2, enemyHitbox.y + enemyHitbox.h+ LIFEBAROFFSET);
    new Sprite(*enemyIndicator, ENEMY_INDICATOR_SPRITE);

    enemyIndicator->box.x -= enemyIndicator->box.w/2;
    enemyIndicator->box.y -= enemyIndicator->box.h;


    Game::GetInstance().GetCurrentState().AddObject(enemyIndicator);
}

void Enemies::DeleteEnemyIndicator() {
    if (enemyIndicator != nullptr) {
        enemyIndicator->RequestDelete();
        enemyIndicator = nullptr;
    }
}


void Enemies::IntentionAnimation(float dt) {
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




void Enemies::CreateIntention() {
    intention = new GameObject(enemyHitbox.x+ enemyHitbox.w/2, enemyHitbox.y);
    new Sprite(*intention, ENEMY_INTENTON_SPRITE);

    intention->box.x -= intention->box.w/2;
    intention->box.y -= intention->box.h/2;
    Game::GetInstance().GetCurrentState().AddObject(intention);
}

void Enemies::DeleteIntention() {
    if (intention != nullptr) {
        intention->RequestDelete();
        intention = nullptr;
    }
}

void Enemies::ApplySkillToEnemy() {
    auto selectedSkill = Skill::selectedSkill;
    Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];

    if(tempSkillInfo.isProtected == Skill::StateProtected::PROTECTED){
        Protected::isProtected = true;
    }
    else if(tempSkillInfo.isProtected == Skill::StateProtected::EXPOSED){
        Protected::isProtected = false;
    } 



    if (tempSkillInfo.attackType == Skill::AttackType::ATTACK_ALL || tempSkillInfo.attackType == Skill::AttackType::DEBUFF_ALL || tempSkillInfo.attackType == Skill::AttackType::BUFF_ALL) {
        ApplySkillToAllEnemies();
    } else {
        ApplySkillToSingleEnemy(tempSkillInfo.damage, tempSkillInfo.tags);
        AP::apCount -= tempSkillInfo.apCost;
        selectedSkill->SkillBack(tempSkillInfo.targetTypeBack);
        selectedSkill->Deselect();
        CombatState::enemiesArrayIS.push_back(id);
        SetupInteractionScreen(tempSkillInfo.attackType, tempSkillInfo.targetTypeAttacker);
        DeleteEnemyIndicator();

    }
    Mother::damageDjinn = tempSkillInfo.damageBack;

}

void Enemies::ApplySkillToSingleEnemy(int damage, std::vector<Tag::Tags> tags) {
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
            auto selectedSkill = Skill::selectedSkill; //poor code, it happens :(
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];
            
                bool tagRampage = false;
                if(tempSkillInfo.targetTypeAttacker == Skill::TargetType::MOTHER){
                    tagRampage= HasTagPlayer(Tag::Tags::RAMPAGE, Mother::tags);
                    if(tagRampage){Mother::activateRampage = true;}
                }
                else if(tempSkillInfo.targetTypeAttacker == Skill::TargetType::DAUGHTER){
                    tagRampage= HasTagPlayer(Tag::Tags::RAMPAGE, Daughter::tags);
                    if(tagRampage){Daughter::activateRampage = true;}
                }

                bool tagWeak = false;
                if(tempSkillInfo.targetTypeAttacker == Skill::TargetType::MOTHER){
                    tagWeak= HasTagPlayer(Tag::Tags::WEAK, Mother::tags);
                    if(tagWeak){Mother::activateWeak = true;}
                }
                else if(tempSkillInfo.targetTypeAttacker == Skill::TargetType::DAUGHTER){
                    tagWeak= HasTagPlayer(Tag::Tags::WEAK, Daughter::tags);
                    if(tagWeak){Daughter::activateWeak = true;}
                }

            if (tagRampage){
                tagMultiplier += 0.25; 
            }
            if (tagWeak){
                tagMultiplier -= 0.25; 
            }

        }
        hp -= damage * tagMultiplier;
            
        ApplyTags(tags);




        if(damage > 0 || dodge){
            lifeBarEnemy->SetCurrentHP(hp);  // Update the enemy's HP bar
        }
        
       
}

void Enemies::ApplySkillToAllEnemies() {
    SkillAllenemies = enemiesCount;
}

void Enemies::ApplyTags(std::vector<Tag::Tags> skillTags) {
    for (auto& tag : skillTags) {
        ActivateTag(tag);
        if (!(std::find(tags.begin(), tags.end(), tag) != tags.end())) {
            tags.push_back(tag);
            auto go_tag = AddObjTag(tag);
            if(tag == Tag::Tags::PROVOKE){
                provokedEnemies++;
            }
        }
        tagCountMap[tag]++;
        // Iterate over the list of weak_ptr to the tag GameObjects
            for (auto& weak_tag : enemytags) {
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
 

void Enemies::ActivateTag(Tag::Tags tag){
    for (auto& weak_tag : enemytags) {
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


std::weak_ptr<GameObject>  Enemies::AddObjTag(Tag::Tags tag){ 
    std::weak_ptr<GameObject> weak_enemy = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);

    GameObject* tagObject = new GameObject();
    new Tag(*tagObject, tag, weak_enemy, tagCountMap[tag]); 


    tagObject->box.x = enemyHitbox.x + TAGS_SPACING_X * tagSpaceCount;
    tagObject->box.y = enemyHitbox.y + enemyHitbox.h + TAGS_SPACING_Y;

    std::weak_ptr<GameObject> go_tag = Game::GetInstance().GetCurrentState().AddObject(tagObject);

    tagSpaceCount += 1;
    enemytags.push_back(go_tag);  

    return go_tag;
}

void Enemies::RemoveOneTagAll() {
    std::vector<Tag::Tags> tagsToRemove;

    for (const auto& tag : tags) {
        if (tagCountMap.find(tag) != tagCountMap.end() && tagCountMap[tag] > 0) {
            

            if(tag == Tag::Tags::CURSE){
                hp -= tagCountMap[tag];
                lifeBarEnemy->SetCurrentHP(hp);
            }

            tagCountMap[tag]--;
            // Iterate over the list of weak_ptr to the tag GameObjects
            auto it = enemytags.begin();
            while (it != enemytags.end()) {
                auto tagGameObject = it->lock();
                if (tagGameObject) {
                    auto tagComponent = tagGameObject->GetComponent("Tag");
                    auto tagComponentPtr = std::dynamic_pointer_cast<Tag>(tagComponent);
                    if (tagComponentPtr && tagComponentPtr->GetTag() == tag) {
                        tagComponentPtr->UpdateQuantity(tagCountMap[tag]);
                        if (tagCountMap[tag] == 0) {
                            tagsToRemove.push_back(tag);
                            tagGameObject->RequestDelete();
                            it = enemytags.erase(it);
                            if(tag == Tag::Tags::PROVOKE){
                                provokedEnemies--;
                            }
                        } else {
                            ++it;
                        }
                    } else {
                        ++it;
                    }
                } else {
                    it = enemytags.erase(it);
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

void Enemies::RecreateTagUI() {
    // Clear all existing tag objects
    for (auto& weak_tag : enemytags) {
        auto tagGameObject = weak_tag.lock();
        if (tagGameObject) {
            tagGameObject->RequestDelete();
        }
    }

    // Clear the list of tag objects
    enemytags.clear();

    tagSpaceCount = 0;

    // Recreate tag objects based on the current tag list
    for (const auto& tag : tags) {
        AddObjTag(tag);
    }
}






bool Enemies::HasTag(Tag::Tags tagToCheck) {
    // Go through the enemy's tag list and check if the desired tag is present. 
    for (const auto& tag : tags) {
        if (tag == tagToCheck) {
            return true; // tag is present
        }
    }
    return false; // tag isnt present.
}

bool Enemies::HasTagPlayer(Tag::Tags tagToCheck, std::vector<Tag::Tags> tags) {
    // Go through the enemy's tag list and check if the desired tag is present. 
    for (const auto& tag : tags) {
        if (tag == tagToCheck) {
            return true; // tag is present
        }
    }
    return false; // tag isnt present.
}


void Enemies::Render() {
    #ifdef DEBUG
    Vec2 center(enemyHitbox.GetCenter());
    SDL_Point points[5];

    Vec2 point = (Vec2(enemyHitbox.x, enemyHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = (Vec2(enemyHitbox.x + enemyHitbox.w, enemyHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[1] = {(int)point.x, (int)point.y};

    point = (Vec2(enemyHitbox.x + enemyHitbox.w, enemyHitbox.y + enemyHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[2] = {(int)point.x, (int)point.y};

    point = (Vec2(enemyHitbox.x, enemyHitbox.y + enemyHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}


Enemies::EnemyId Enemies::GetId() {
    return id;
}

bool Enemies::Is(std::string type) {
    return (type == "Enemies"); 
}

// Implement the InitializeEnemyInfoMap function to populate enemy information 
void Enemies::InitializeEnemyInfoMap() { 

    /*
    Cultista Verde (Debuffer):
        6 ~10 HP
        Cut: Deal 6 damage
        Whip: Deal 3 damage; Apply 1 Frail to target.
        Evil Chant: Apply 2 weak to target

    */
    enemyInfoMap[CultistGreen] = { std::make_pair(6, 10), {}, "Green Cultist", CultistGreen_SPRITE, CultistGreen_SPRITE_ATK, CultistGreen_SPRITE_DFS, {Skill::E_Cut, Skill::E_Whip, Skill::E_Evil_Chant} };
    
    
    /*
    Cultista Vermelho (Debuffer):
        13 ~ 19 HP
        Shiv: Deal 9 damage 
        Whip: Deal 3 damage; Apply 2 Frail to target.
        Evil Chant: Apply 2 weak to target


    */
    enemyInfoMap[CultistRed] = { std::make_pair(13, 19), {}, "Red Cultist", CultistRed_SPRITE, CultistRed_SPRITE_ATK, CultistRed_SPRITE_DFS, {Skill::E_Shiv, Skill::E_Whip, Skill::E_Evil_Chant} };
    
    /*
    Cultista Roxo (Damage Dealer):
        18 ~ 28 HP
        9 Tails: Deal 12 damage
        Lash: Deal 9 damage
        Deafening Whispers: Deal 3 damage; Apply 2 Frail to target

    */
    enemyInfoMap[CultistPurple] = { std::make_pair(15, 28), {}, "Purple Cultist", CultistPurple_SPRITE, CultistPurple_SPRITE_ATK, CultistPurple_SPRITE_DFS, {Skill::E_Tentacle_Strike, Skill::E_Bite, Skill::E_Whip} };
    
    
   /*
    Pintinho (Offensive Support)
        17 ~ 25HP
        Beak: Deal 3 damage
        Unnerving Presence: Apply 2 Weak and 2 Vulnerable to target
        Guttural Scream: Apply 2 rampage to all allies


    */
    enemyInfoMap[Parakeet] = { std::make_pair(17, 25), {}, "Parakeet", Parakeet_SPRITE, Parakeet_SPRITE_ATK, Parakeet_SPRITE_DFS, {Skill::E_Beak, Skill::E_Unnerving_Presence, Skill::E_Guttural_Scream} };

    /*
    Bicho da Lâmpada (Boss)
        60HP
        Infernal Scream: Apply 3 Weak and 3 Vulnerable to target
        Infernal Skull: Deal 5 damage; Gain 3 Resilience
        Enrage: Aplly 3 rampage to all allies
        Impale: Deal 15 damage


    */
    enemyInfoMap[Radog] = { std::make_pair(60, 60), {}, "Radog", radog_SPRITE, radog_SPRITE_ATK, radog_SPRITE_DFS, {Skill::E_Infernal_Scream, Skill::E_Infernal_Skull, Skill::E_Enrage, Skill::E_Impale} };

    /*    
    Estátua Preta (Glass Cannon)
        10 ~ 15 HP
        Freezing Stare: Deal 8 Damage
        Empower: Deal 3 damage; Gain 2 Rampage


    */
    enemyInfoMap[CatStone] = { std::make_pair(10, 15), {}, "Cat Stone", CatStone_SPRITE, CatStone_SPRITE_ATK, CatStone_SPRITE_DFS, {Skill::E_Freezing_Stare, Skill::E_Empower, Skill::E_Empower} };
    
    /*    
    Estátua Dourada (Glass Cannon)
        18 ~ 25 HP
        Take Soul: Deal 15 Damage
        Empower: Deal 3 damage; Gain 2 Rampage

    */
    enemyInfoMap[CatGold] = { std::make_pair(18, 25), {}, "Cat Gold", CatGold_SPRITE, CatGold_SPRITE_ATK, CatGold_SPRITE_DFS, {Skill::E_Take_Soul, Skill::E_Empower, Skill::E_Empower} };
    

    /*    
    Sapo Verde Mae (Defensive Support)
        7~12HP
        Bubble Shield: Apply 3 resilience to all allies
        Tongue Strike: Deal 3 damage; Apply 3 weak to target and 2 curse
        Toxic Sludge: Apply 4 curse to one enemy
    */
    enemyInfoMap[FrogMom] = { std::make_pair(7, 12), {}, "Frog Mom", frogMom_SPRITE, frogMom_SPRITE_ATK, frogMom_SPRITE_DFS, {Skill::E_Bubble_Shield, Skill::E_Tongue_Strike, Skill::E_Toxic_Sludge} };
    

    /*    
    Sapo Vermelho PAI(Brute):
        20 ~ 30 HP
        Lick: Deal 6 damage
        Digest: Gain 3 resilience
        Inflate: Gain 3 provoke
    */
    enemyInfoMap[FrogDad] = { std::make_pair(20, 30), {}, "Frog Dad", frogDad_SPRITE, frogDad_SPRITE_ATK, frogDad_SPRITE_DFS, {Skill::E_Lick, Skill::E_Digest, Skill::E_Inflate} };
    

    /*    
    Aranha (Boss)
        60HP
        Infernal Scream: Apply 3 Weak and 3 Vulnerable to target
        Exoskeleton: Deal 5 damage; Gain 3 Resilience
        Enrage: Gain 3 rampage
        Impale: Deal 15 damage

    */
    enemyInfoMap[Spider] = { std::make_pair(60, 60), {}, "Enemy 2", ENEMY4_SPRITE, ENEMY4_SPRITE_ATK, ENEMY4_SPRITE_DFS, {Skill::E1_Skill1, Skill::E1_Skill2, Skill::E1_Skill3} };

}
   