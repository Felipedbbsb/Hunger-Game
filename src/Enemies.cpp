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
#include <algorithm> 

#ifdef DEBUG
#include "Camera.h"
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
    intentionTimer()
    {  
        EnemyInfo& enemyInfo = enemyInfoMap[id];
        hp = enemyInfo.hp;
        tags = enemyInfo.tags;
        name = enemyInfo.name;
        iconPath = enemyInfo.iconPath;
        skills = enemyInfo.skills;
 
    
        enemiesCount += 1;    //one more enemy 
}
 
void Enemies::Start() {
    Sprite* enemies_spr = new Sprite(associated, iconPath); 
    associated.AddComponent(std::shared_ptr<Sprite>(enemies_spr)); 
    associated.box.y -= associated.box.h;

    //===================================Hitbox==================================
    enemyHitbox = Rect(associated.box.x, associated.box.y, 130, associated.box.h);

    associated.box.x -= (associated.box.w - enemyHitbox.w )/2;

    //==================================LifeBar====================================
    lifeBarEnemy = new LifeBar(associated, hp, hp, enemyHitbox.w, enemyHitbox.x); //width from hitbox
    associated.AddComponent(std::shared_ptr<LifeBar>(lifeBarEnemy));

    //If enemies starts with tags
    ApplyTags(tags);

    lifeBarEnemy->SetCurrentHP(hp);

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

    enemiesCount -= 1;
    enemiesToAttack-= 1;

     // Remove the enemy from the enemiesArray vector
    enemiesArray.erase(std::remove_if(enemiesArray.begin(), enemiesArray.end(),
        [this](const std::weak_ptr<GameObject>& enemy) {
            return enemy.lock().get() == &associated;
        }), enemiesArray.end());
}

void Enemies::Update(float dt) {
    if(CombatState::InteractionSCreenActivate){
        return;
    }

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


    // Check if the enemy's HP is zero or below and request deletion
    if (hp <= 0) {
        associated.RequestDelete();
        return; // Early exit if the enemy is no longer alive

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

                // Check if the mouse is over the enemy and left mouse button is pressed
                if (enemyHitbox.Contains(mousePos.x, mousePos.y) && inputManager.MousePress(LEFT_MOUSE_BUTTON)) {
                    if (!provokedEnemies ||  (provokedEnemies && HasTag(Tag::Tags::PROVOKE))){
                        //checks if any enemie has provoke
                        ApplySkillToEnemy();

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
                
                // Now, the selected skill is in the last position of the vector
                Skill::selectedSkill = new Skill(associated, selectedSkillId, nullptr);
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
                        CombatState::enemiesArrayIS.push_back(id);
                        Skill::HasTagRampageOrWeak ={hasRampage, hasWeak};
                    }    
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
 
void Enemies::CreateEnemyIndicator() {
    enemyIndicator = new GameObject(enemyHitbox.x + enemyHitbox.w/2, enemyHitbox.y + enemyHitbox.h+ LIFEBAROFFSET);
    Sprite* enemyIndicator_spr = new Sprite(*enemyIndicator, ENEMY_INDICATOR_SPRITE);

    enemyIndicator->box.x -= enemyIndicator->box.w/2;
    enemyIndicator->box.y -= enemyIndicator->box.h;

    enemyIndicator->AddComponent(std::make_shared<Sprite>(*enemyIndicator_spr));
    Game::GetInstance().GetCurrentState().AddObject(enemyIndicator);
}

void Enemies::DeleteEnemyIndicator() {
    if (enemyIndicator != nullptr) {
        enemyIndicator->RequestDelete();
        enemyIndicator = nullptr;
    }
}


void Enemies::CreateIntention() {
    intention = new GameObject(enemyHitbox.x+ enemyHitbox.w/2, enemyHitbox.y);
    Sprite* intention_spr = new Sprite(*intention, ENEMY_INTENTON_SPRITE);
    intention->AddComponent(std::make_shared<Sprite>(*intention_spr));
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



}

void Enemies::ApplySkillToSingleEnemy(int damage, std::vector<Tag::Tags> tags) {
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
        lifeBarEnemy->SetCurrentHP(hp);  // Update the enemy's HP bar
       
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
    Tag* tag_behaviour = new Tag(*tagObject, tag, weak_enemy, tagCountMap[tag]);
    tagObject->AddComponent(std::shared_ptr<Tag>(tag_behaviour));

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
    // Populate the map with enemy information during initialization.
    enemyInfoMap[ENEMY1] = { 10, {}, "Enemy 1", ENEMY1_SPRITE, {Skill::E1_Skill1, Skill::E1_Skill2, Skill::E1_Skill3} };
    enemyInfoMap[ENEMY2] = { 20, {}, "Enemy 2", ENEMY2_SPRITE, {Skill::E2_Skill1, Skill::E2_Skill2, Skill::E2_Skill3} };
    enemyInfoMap[ENEMY3] = { 30, {}, "Enemy 1", ENEMY3_SPRITE, {Skill::E3_Skill1, Skill::E3_Skill2, Skill::E3_Skill1} };
    enemyInfoMap[ENEMY4] = { 100, {}, "Enemy 2", ENEMY4_SPRITE, {Skill::E1_Skill1, Skill::E1_Skill2, Skill::E1_Skill3} };
}
 