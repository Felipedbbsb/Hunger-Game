#include "Enemies.h"
#include "InputManager.h"
#include "Skill.h" // Include the Skill header to access selectedSkill and SkillInfo
#include "LifeBar.h"
#include "Game.h"

// Initialize the static enemyInfoMap
std::map<Enemies::EnemyId, Enemies::EnemyInfo> Enemies::enemyInfoMap;

Enemies::Enemies(GameObject& associated, EnemyId id)
    : Component::Component(associated), 
    id(id),
    lifeBarEnemy(nullptr)
    {
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
    Vec2 mousePos(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());

    // Check if the enemy's HP has reached zero or below and handle it accordingly
    if (enemyInfoMap[id].hp <= 0) {
        associated.RequestDelete(); 
    }


    if (associated.box.Contains(mousePos.x, mousePos.y)) {
        if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) {
            // Check if a skill is selected and it's not the current enemy 
            if (Skill::selectedSkill != nullptr) {
                // Get the SkillInfo of the selected skill
                Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[Skill::selectedSkill->GetId()];
                
                std::cout << "Enemy HP before: " << enemyInfoMap[id].hp << std::endl;

                // Calculate damage to the enemy (e.g., reduce enemy's HP)
                int damage = tempSkillInfo.damage;
                enemyInfoMap[id].hp -= damage;
                

                // Atualize a barra de vida (LifeBar) associada a este inimigo
                lifeBarEnemy->SetCurrentHP(enemyInfoMap[id].hp);


                // Print the damage caused by the skill and the enemy's remaining HP
                std::cout << "Selected Skill Damage: " << damage << std::endl;
                std::cout << "Enemy HP now: " << enemyInfoMap[id].hp << std::endl;

                // Print the ID of each component
                std::cout << "Selected Skill ID: " << Skill::selectedSkill->GetId() << std::endl;
                std::cout << "Enemy ID: " << id << std::endl;


                // Deselect the previously selected skill
                Skill::selectedSkill->Deselect();
            }

            
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
    enemyInfoMap[ENEMY1] = { 30, {"tag1", "tag2"}, "Enemy 1", ENEMY1_SPRITE };
    enemyInfoMap[ENEMY2] = { 20, {"tag3", "tag4"}, "Enemy 2", ENEMY2_SPRITE };
    enemyInfoMap[ENEMY3] = { 30, {"tag1", "tag2"}, "Enemy 1", ENEMY3_SPRITE };
    enemyInfoMap[ENEMY4] = { 100, {"tag3", "tag4"}, "Enemy 2", ENEMY4_SPRITE };
}
