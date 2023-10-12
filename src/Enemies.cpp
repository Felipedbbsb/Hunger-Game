#include "Enemies.h"
#include "InputManager.h"
#include "Skill.h" // Include the Skill header to access selectedSkill and SkillInfo
#include "LifeBar.h"
#include "Game.h"

// Initialize the static enemyInfoMap
std::map<Enemies::EnemyId, Enemies::EnemyInfo> Enemies::enemyInfoMap;

std::vector<std::shared_ptr<GameObject>> tags;


Enemies::Enemies(GameObject& associated, EnemyId id, std::vector<std::shared_ptr<Enemies>> enemiesArray)
    : Component::Component(associated), 
    enemyIndicator(nullptr),
    id(id),
    lifeBarEnemy(nullptr),
    enemiesArray(enemiesArray){
}

void Enemies::Start() {
    std::string spriteEnemy;
    // Use enemyInfoMap para obter informações do inimigo com base na ID
    const EnemyInfo& enemyInfo = enemyInfoMap[id];

    spriteEnemy = enemyInfo.iconPath; 

    // Adicione o sprite do inimigo 
    Sprite* enemies_spr = new Sprite(associated, spriteEnemy);
    enemies_spr->SetScale(0.15, 0.15);
    associated.AddComponent(std::shared_ptr<Sprite>(enemies_spr));

    // Inicialize a LifeBar
    lifeBarEnemy = new LifeBar(associated, enemyInfo.hp, enemyInfo.hp, associated.box.w);
    associated.AddComponent(std::shared_ptr<LifeBar>(lifeBarEnemy));

}

Enemies::~Enemies() { 
}

void Enemies::Update(float dt) {
    auto& inputManager = InputManager::GetInstance();// Get the input manager instanc
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());

    auto& enemyInfo = enemyInfoMap[id];// Get the enemy's information
    
    auto selectedSkill = Skill::selectedSkill;// Get the selected skill

    // Check if the enemy's HP is zero or below and request deletion
    if (enemyInfo.hp <= 0) {
        associated.RequestDelete();
    }

    // Check if a skill is selected
    if (selectedSkill != nullptr) {
        if (enemyIndicator == nullptr) {
            CreateEnemyIndicator();// Create an enemy indicator if it doesn't exist
        }
    } 
    else {      
        DeleteEnemyIndicator();// Delete the enemy indicator if it exists
    }
    // Deselect the skill on right mouse button press
    if (inputManager.MousePress(RIGHT_MOUSE_BUTTON) && selectedSkill != nullptr) {
        selectedSkill->Deselect();
    }

    // Check if the mouse is over the enemy and left mouse button is pressed
    if (associated.box.Contains(mousePos.x, mousePos.y) && inputManager.MousePress(LEFT_MOUSE_BUTTON)) {
        if (selectedSkill != nullptr) {
            ApplySkillToEnemy(id);
        }
    }
    // Update the enemy's HP bar
    lifeBarEnemy->SetCurrentHP(enemyInfo.hp);

    RenderTags();
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
        ApplySkillToAllEnemies(tempSkillInfo.damage);
    } else {
        auto& enemyInfo = Enemies::enemyInfoMap[enemyId];
        enemyInfo.hp -= tempSkillInfo.damage;
    }

    selectedSkill->Deselect();
}

void Enemies::ApplySkillToAllEnemies(int damage) {
    for (auto& enemy : enemiesArray) {
        Enemies::EnemyId enemyId = enemy->GetId();

        if (Enemies::enemyInfoMap.find(enemyId) != Enemies::enemyInfoMap.end()) {
            Enemies::EnemyInfo& enemyInfo = Enemies::enemyInfoMap[enemyId];
            enemyInfo.hp -= damage;
        }
    }
}

void Enemies::RenderTags() {
    if (tags.empty()) {
        float tagSpacing = 50.0f;
        Enemies::EnemyInfo& enemyInfo = Enemies::enemyInfoMap[id];
        float currentX = associated.box.x;

        for (auto tag : enemyInfo.tags) {
            std::string spriteTag;

            switch (tag) {
                case Skill::SkillsTags::RESILIENCE: spriteTag = TAG_RESILIENCE_SPRITE; break;
                case Skill::SkillsTags::DODGE: spriteTag = TAG_DODGE_SPRITE; break;
                case Skill::SkillsTags::PROVOKE: spriteTag = TAG_PROVOKE_SPRITE; break;
                case Skill::SkillsTags::VULNERABLE: spriteTag = TAG_VULNERABLE_SPRITE; break;
                case Skill::SkillsTags::WEAK: spriteTag = TAG_WEAK_SPRITE; break;
                case Skill::SkillsTags::RAMPAGE: spriteTag = TAG_RAMPAGE_SPRITE; break;
                case Skill::SkillsTags::PROTECTED: spriteTag = TAG_PROTECTED_SPRITE; break;
            }

            // Crie um objeto tag com um Sprite usando std::make_shared
            auto tagObject = std::make_shared<GameObject>();
            auto tag_spr = std::make_shared<Sprite>(*tagObject, spriteTag);
            tag_spr->SetScale(0.1, 0.1);
            tagObject->AddComponent(tag_spr);
            tagObject->box.x = currentX;
            tagObject->box.y = associated.box.y + associated.box.h;
            tags.push_back(tagObject);
            Game::GetInstance().GetCurrentState().AddObject(tagObject.get());
            currentX += tagSpacing;
        }
    }
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
