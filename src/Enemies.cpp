#include "Enemies.h"
#include "InputManager.h"
#include "Skill.h" // Include the Skill header to access selectedSkill and SkillInfo
#include "LifeBar.h"
#include "Tag.h"
#include "Game.h"
#include "AP.h" 
#include "GameData.h" 
#include <algorithm> 

#ifdef DEBUG
#include "Camera.h"
#include "Game.h"

#include <SDL2/SDL.h>
#endif //DEBUG

// Initialize the static enemyInfoMap
std::map<Enemies::EnemyId, Enemies::EnemyInfo> Enemies::enemyInfoMap;

std::vector<std::shared_ptr<Enemies>> Enemies::enemiesArray;

int Enemies::enemiesCount = 0;

int Enemies::SkillAllenemies = 0;//how many left enemies to receive skill effects

int Enemies::provokedEnemies = 0;//how many left enemies has provoke

int Enemies::enemiesToAttack = 0;//how many left enemies to attack

bool Enemies::enemyAttacking = false;
 
Enemies::Enemies(GameObject& associated, EnemyId id)
    : Component::Component(associated), 
    enemyIndicator(nullptr),
    id(id), 
    lifeBarEnemy(nullptr), 
    tagSpaceCount(0),
    thisEnemyAttacked(false)
    {  

        EnemyInfo& enemyInfo = enemyInfoMap[id];
        hp = enemyInfo.hp;
        tags = enemyInfo.tags;
        name = enemyInfo.name;
        iconPath = enemyInfo.iconPath;
        skills = enemyInfo.skills;
 
        // Create a shared pointer for the current Enemies object
        std::shared_ptr<Enemies> sharedThis = std::shared_ptr<Enemies>(this);

        // Create a weak pointer from the shared pointer and add it to the enemiesArray
        std::weak_ptr<Enemies> weakThis = sharedThis;
        enemiesArray.push_back(sharedThis);
    
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

    enemiesCount -= 1;
}

void Enemies::Update(float dt) {
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());

    auto selectedSkill = Skill::selectedSkill;
    
//=============================//=============================//=============================//=============================
    //Iterator for all skill types, counts number of left enemies to receive skill from player
    if(SkillAllenemies > 0 && GameData::playerTurn == true){
        Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];
        if (!provokedEnemies ||  (provokedEnemies != 0  && HasTag(Tag::Tags::PROVOKE))){
            ApplySkillToSingleEnemy(tempSkillInfo.damage, tempSkillInfo.tags);
        }
        
        SkillAllenemies -= 1; //less one enemy to receive skill
        if(SkillAllenemies == 0){ //no more enemies
            AP::apCount -= tempSkillInfo.apCost;
            selectedSkill->SkillBack(tempSkillInfo.targetTypeBack);
            selectedSkill->Deselect();
        }
    }



//=============================//=============================//=============================//=============================


    // Check if the enemy's HP is zero or below and request deletion
    if (hp <= 0) {
        DeleteEnemyIndicator();
        associated.RequestDelete();
        return; // Early exit if the enemy is no longer alive

    } 
    
//=============================//=============================//=============================//=============================
    //----Intention manager----

//=============================//=============================//=============================//=============================
    //PLAYER TURN
    if(GameData::playerTurn == true){

        thisEnemyAttacked = false; //reset 

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
        

        std::cout << "chico mkjilo2222222" <<!enemyAttacking << !thisEnemyAttacked<<std::endl;
        //=============================Targeted skill sector=============================
        //Sector to manipulate interections involving mother being attacked
        if(!enemyAttacking && !thisEnemyAttacked && Skill::selectedSkillEnemy == nullptr){
    
            // Check if the enemy has at least two skills
            if (skills.size() >= 2) {
                int randomSkillIndex = rand() % 2; //chooses between first or second skill
                Skill::SkillId selectedSkillId = skills[randomSkillIndex];
                
                // Moves the selected skill to the end of the vector
                std::swap(skills[randomSkillIndex], skills.back());
                
                // Now, the selected skill is in the last position of the vector
                Skill::selectedSkillEnemy = new Skill(associated, selectedSkillId, nullptr);
            }
            
            enemyAttacking = true;
            thisEnemyAttacked = true;
            enemiesToAttack -= 1;

            //=============================Skill back sector=================================
            //Sector to manipulate interections involving enemies being attacked
            if(Skill::selectedSkillEnemy != nullptr){
                Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[Skill::selectedSkillEnemy->GetId()];
                if(tempSkillInfo.attackTypeBack == Skill::AttackType::BUFF_INDIVIDUAL
                || tempSkillInfo.attackTypeBack == Skill::AttackType::BUFF_ALL){
                    ApplySkillToSingleEnemy(tempSkillInfo.damageBack, tempSkillInfo.tagsBack);
                    
                }
            }   

        }


        //ApplySkillToAllEnemies()
        //=============================Skill buff sector==============================
        //TODO SKILL BUFF DEFENSE


        

    }

    

}
 
void Enemies::CreateEnemyIndicator() {
    enemyIndicator = new GameObject(enemyHitbox.x, enemyHitbox.y + enemyHitbox.h);
    Sprite* enemyIndicator_spr = new Sprite(*enemyIndicator, ENEMY_INDICATOR_SPRITE);

    // Scale the enemy indicator
    float percentageEnemyWidth = enemyHitbox.w / enemyIndicator->box.w;
    enemyIndicator_spr->SetScale(percentageEnemyWidth, 1);
    enemyIndicator->AddComponent(std::make_shared<Sprite>(*enemyIndicator_spr));
    Game::GetInstance().GetCurrentState().AddObject(enemyIndicator);
}

void Enemies::DeleteEnemyIndicator() {
    if (enemyIndicator != nullptr) {
        enemyIndicator->RequestDelete();
        enemyIndicator = nullptr;
    }
}

void Enemies::ApplySkillToEnemy() {
    auto selectedSkill = Skill::selectedSkill;
    Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];

    if (tempSkillInfo.attackType == Skill::AttackType::ATTACK_ALL || tempSkillInfo.attackType == Skill::AttackType::DEBUFF_ALL) {
        ApplySkillToAllEnemies();
    } else {
        ApplySkillToSingleEnemy(tempSkillInfo.damage, tempSkillInfo.tags);
        AP::apCount -= tempSkillInfo.apCost;
        selectedSkill->SkillBack(tempSkillInfo.targetTypeBack);
        selectedSkill->Deselect();
    }
}

void Enemies::ApplySkillToSingleEnemy(int damage, std::vector<Tag::Tags> tags) {
        float tagMultiplier = 1; //multiplier without tags
        if (HasTag(Tag::Tags::RESILIENCE)){
            ActivateTag(Tag::Tags::RESILIENCE);
            tagMultiplier -= 0.5; 
        }
        if (HasTag(Tag::Tags::VULNERABLE)){
            ActivateTag(Tag::Tags::VULNERABLE);
            tagMultiplier += 0.5; 
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
        if (tagCountMap.find(tag) != tagCountMap.end()) {
            // The tag already exists, increment the counter
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
        } else {
            // The tag doesn't exist in the map, add it with a counter of 1
            tagCountMap[tag] = 1;
            tags.push_back(tag);
            auto go_tag = AddObjTag(tag);
            
            if(tag == Tag::Tags::PROVOKE){
                provokedEnemies++;
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
    Tag* tag_behaviour = new Tag(*tagObject, tag, weak_enemy, 1);
    tagObject->AddComponent(std::shared_ptr<Tag>(tag_behaviour));

    tagObject->box.x = enemyHitbox.x + TAGS_SPACING_X * tagSpaceCount;
    tagObject->box.y = enemyHitbox.y + enemyHitbox.h + TAGS_SPACING_Y;
    std::weak_ptr<GameObject> go_tag = Game::GetInstance().GetCurrentState().AddObject(tagObject);

    tagSpaceCount += 1;
    enemytags.push_back(go_tag);  

    return go_tag;
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
    enemyInfoMap[ENEMY1] = { 10, {Tag::Tags::PROVOKE}, "Enemy 1", ENEMY1_SPRITE, {Skill::E1_Skill1, Skill::E1_Skill2, Skill::E1_Skill3} };
    enemyInfoMap[ENEMY2] = { 20, {}, "Enemy 2", ENEMY2_SPRITE, {Skill::E1_Skill1, Skill::E1_Skill2, Skill::E1_Skill3} };
    enemyInfoMap[ENEMY3] = { 30, {}, "Enemy 1", ENEMY3_SPRITE, {Skill::E1_Skill1, Skill::E1_Skill2, Skill::E1_Skill3} };
    enemyInfoMap[ENEMY4] = { 100, {Tag::Tags::PROVOKE}, "Enemy 2", ENEMY4_SPRITE, {Skill::E1_Skill1, Skill::E1_Skill2, Skill::E1_Skill3} };
}
 