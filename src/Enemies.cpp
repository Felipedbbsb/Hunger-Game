#include "Enemies.h"
#include "InputManager.h"
#include "Skill.h" // Include the Skill header to access selectedSkill and SkillInfo
#include "LifeBar.h"
#include "Tag.h"
#include "Game.h"
#include <algorithm> 
// Initialize the static enemyInfoMap
std::map<Enemies::EnemyId, Enemies::EnemyInfo> Enemies::enemyInfoMap;

std::vector<std::shared_ptr<Enemies>> Enemies::enemiesArray;

std::vector<std::shared_ptr<GameObject>> enemytags;


 
Enemies::Enemies(GameObject& associated, EnemyId id)
    : Component::Component(associated), 
    enemyIndicator(nullptr),
    id(id), 
    lifeBarEnemy(nullptr),
    tagSpaceCount(0)
    { 

        EnemyInfo& enemyInfo = enemyInfoMap[id];
        hp = enemyInfo.hp;
        tags = enemyInfo.tags;
        name = enemyInfo.name;
        iconPath = enemyInfo.iconPath;

        auto sharedThis = std::shared_ptr<Enemies>(this);
        // Adicione o std::shared_ptr ao vetor de inimigos
        enemiesArray.push_back(sharedThis);

}
 
void Enemies::Start() {
    Sprite* enemies_spr = new Sprite(associated, iconPath);
    enemies_spr->SetScale(0.15, 0.15);
    associated.AddComponent(std::shared_ptr<Sprite>(enemies_spr));

    lifeBarEnemy = new LifeBar(associated, hp, hp, associated.box.w);
    associated.AddComponent(std::shared_ptr<LifeBar>(lifeBarEnemy));

    //If enemies starts with tags
    for (auto tag : tags) {
            AddObjTag(tag);
        }
}

Enemies::~Enemies() { 
    std::cout << "teste" << std::endl; 
    for (auto& tag : enemytags) {
        if (!tag->IsDead()) {
            tag->RequestDelete();
        }
    }
    enemytags.clear();
}

void Enemies::Update(float dt) {
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());

    auto selectedSkill = Skill::selectedSkill;

     // Check if the enemy's HP is zero or below and request deletion
    if (hp <= 0) {
        DeleteEnemyIndicator();
        for (auto& tag : enemytags) {
            if (!tag->IsDead()) {
                tag->RequestDelete();
            }
        }
        associated.RequestDelete();
    } 
    
    // Check if a skill is selected
    else if (selectedSkill) {
        if (enemyIndicator == nullptr) {
            CreateEnemyIndicator();// Create an enemy indicator if it doesn't exist
        }
    } 
    
    else {
        DeleteEnemyIndicator();// Delete the enemy indicator if it exists
    }

    // Check if the mouse is over the enemy and left mouse button is pressed
    if (inputManager.MousePress(RIGHT_MOUSE_BUTTON) && selectedSkill) {
        selectedSkill->Deselect();
    }

    // Check if the mouse is over the enemy and left mouse button is pressed
    if (associated.box.Contains(mousePos.x, mousePos.y) && inputManager.MousePress(LEFT_MOUSE_BUTTON) && selectedSkill) {
        ApplySkillToEnemy(id);
    }

    lifeBarEnemy->SetCurrentHP(hp);  // Update the enemy's HP bar
}
 

void Enemies::CreateEnemyIndicator() {
    enemyIndicator = new GameObject(associated.box.x, associated.box.y + associated.box.h);
    Sprite* enemyIndicator_spr = new Sprite(*enemyIndicator, ENEMY_INDICATOR_SPRITE);

    // Scale the enemy indicator
    float percentageEnemyWidth = associated.box.w / enemyIndicator->box.w;
    enemyIndicator_spr->SetScale(percentageEnemyWidth, 0.2);
    enemyIndicator->AddComponent(std::make_shared<Sprite>(*enemyIndicator_spr));
    Game::GetInstance().GetCurrentState().AddObject(enemyIndicator);
}

void Enemies::DeleteEnemyIndicator() {
    if (enemyIndicator != nullptr) {
        enemyIndicator->RequestDelete();
        enemyIndicator = nullptr;
    }
}


void Enemies::ApplySkillToEnemy(EnemyId enemyId) {
    auto selectedSkill = Skill::selectedSkill;
    Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];

    if (tempSkillInfo.attackType == Skill::AttackType::ATTACK_ALL) {
        //ApplySkillToAllEnemies(tempSkillInfo.damage, tempSkillInfo.tags);
        ApplySkillToSingleEnemy(tempSkillInfo);
    } else {
        ApplySkillToSingleEnemy(tempSkillInfo);
    }

    selectedSkill->Deselect();
} 
 
void Enemies::ApplySkillToAllEnemies(int damage, std::vector<Skill::SkillsTags>& skillTags) {
    //for (auto& enemy : enemiesArray) {
    //    auto enemyId = enemy->GetId();
    //    auto selectedSkill = Skill::selectedSkill;
    //    Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];

    //    if (Enemies::enemyInfoMap.find(enemyId) != Enemies::enemyInfoMap.end()) {
   //         auto& enemyInfo = Enemies::enemyInfoMap[enemyId];
    //        ApplySkillToSingleEnemy(enemyInfo, tempSkillInfo);
    //    }
    //}
}

void Enemies::ApplySkillToSingleEnemy( Skill::SkillInfo& skillInfo) {
    hp -= skillInfo.damage;
    ApplyTags(skillInfo.tags);
}

void Enemies::ApplyTags(std::vector<Skill::SkillsTags> skillTags) {
    for (auto& tag : skillTags) {
        tags.push_back(tag);
        AddObjTag(tag);
    }
    
}


void Enemies::AddObjTag(Skill::SkillsTags tag){ 
    auto tagObject = std::make_shared<GameObject>();
    auto tag_behaviour = std::make_shared<Tag>(*tagObject, tag);
    tagObject->AddComponent(tag_behaviour);

    tagObject->box.x = associated.box.x + TAGS_SPACING * tagSpaceCount;
    tagObject->box.y = associated.box.y + associated.box.h;
    Game::GetInstance().GetCurrentState().AddObject(tagObject.get());

    tagSpaceCount += 1;
    enemytags.push_back(tagObject);
    std::cout << "render" << std::endl;
        
}

void Enemies::Render() {
    
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
    enemyInfoMap[ENEMY1] = { 10, {Skill::SkillsTags::VULNERABLE}, "Enemy 1", ENEMY1_SPRITE };
    enemyInfoMap[ENEMY2] = { 20, {Skill::SkillsTags::RESILIENCE, Skill::SkillsTags::VULNERABLE}, "Enemy 2", ENEMY2_SPRITE };
    enemyInfoMap[ENEMY3] = { 30, {Skill::SkillsTags::DODGE}, "Enemy 1", ENEMY3_SPRITE };
    enemyInfoMap[ENEMY4] = { 100, {Skill::SkillsTags::RAMPAGE, Skill::SkillsTags::WEAK}, "Enemy 2", ENEMY4_SPRITE };
}
