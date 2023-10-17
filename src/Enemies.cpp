#include "Enemies.h"
#include "InputManager.h"
#include "Skill.h" // Include the Skill header to access selectedSkill and SkillInfo
#include "LifeBar.h"
#include "Tag.h"
#include "Game.h"
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

}

Enemies::~Enemies() { 
    for (int i = enemytags.size() - 1; i >= 0; i--) { //remove enemies tags
            enemytags.erase(enemytags.begin() + i);
        }

    DeleteEnemyIndicator();
    enemiesCount -= 1;
}

void Enemies::Update(float dt) {
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());

    auto selectedSkill = Skill::selectedSkill;

    //Iterator for all skill types, counts number of left enemies to receive skill
    if(SkillAllenemies > 0){
        Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];
        ApplySkillToSingleEnemy(tempSkillInfo);
        
        SkillAllenemies -= 1; //less one enemy to receive skill
        if(SkillAllenemies == 0){ //no more enemies
            selectedSkill->Deselect();
        }
    }

    // Check if the enemy's HP is zero or below and request deletion
    if (hp <= 0) {
        DeleteEnemyIndicator();
        associated.RequestDelete();
        return; // Early exit if the enemy is no longer alive

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
    if (enemyHitbox.Contains(mousePos.x, mousePos.y) && inputManager.MousePress(LEFT_MOUSE_BUTTON) && selectedSkill) {
        ApplySkillToEnemy();
    }

    lifeBarEnemy->SetCurrentHP(hp);  // Update the enemy's HP bar
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

    if (tempSkillInfo.attackType == Skill::AttackType::ATTACK_ALL) {
        ApplySkillToAllEnemies();
    } else {
        ApplySkillToSingleEnemy(tempSkillInfo);
        selectedSkill->Deselect();
    }
}

void Enemies::ApplySkillToSingleEnemy(Skill::SkillInfo& skillInfo) {
    hp -= skillInfo.damage;
    ApplyTags(skillInfo.tags);
}

void Enemies::ApplySkillToAllEnemies() {
    SkillAllenemies = enemiesCount;
}

void Enemies::ApplyTags(std::vector<Skill::SkillsTags> skillTags) {
    for (auto& tag : skillTags) {
        if (tagCountMap.find(tag) != tagCountMap.end()) {
                // A tag já existe, incrementa o contador
                tagCountMap[tag]++;
                for (auto& weak_tag : enemytags) {
                    auto tagGameObject = weak_tag.lock();  // Obtenha o objeto GameObject
                    if (tagGameObject) {
                        // Tente recuperar o componente "Tag"
                        auto tagComponent = tagGameObject->GetComponent("Tag");
                        auto tagComponentPtr = std::dynamic_pointer_cast<Tag>(tagComponent);;
                        if (tagComponentPtr) {
                            // Agora, você pode acessar a propriedade "tag" do componente "Tag"
                            if (tagComponentPtr->GetTag() == tag) {
                                tagComponentPtr->UpdateQuantity(tagCountMap[tag]);
                            }
                        }
                    } 
                }
            } else {
                // A tag não existe no mapa, adiciona com contador 1
                tagCountMap[tag] = 1;
                tags.push_back(tag);
                auto go_tag = AddObjTag(tag);
                
            }

        
    }
}
 
std::weak_ptr<GameObject>  Enemies::AddObjTag(Skill::SkillsTags tag){ 
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
    enemyInfoMap[ENEMY1] = { 10, {}, "Enemy 1", ENEMY1_SPRITE };
    enemyInfoMap[ENEMY2] = { 20, {Skill::SkillsTags::RESILIENCE, Skill::SkillsTags::VULNERABLE}, "Enemy 2", ENEMY2_SPRITE };
    enemyInfoMap[ENEMY3] = { 30, {Skill::SkillsTags::DODGE}, "Enemy 1", ENEMY3_SPRITE };
    enemyInfoMap[ENEMY4] = { 100, {Skill::SkillsTags::RAMPAGE, Skill::SkillsTags::WEAK}, "Enemy 2", ENEMY4_SPRITE };
}
