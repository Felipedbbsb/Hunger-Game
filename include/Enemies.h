#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "GameObject.h"
#include "Component.h"
#include "Sprite.h"
#include "LifeBar.h"
#include "Skill.h"

#define ENEMY_INDICATOR_SPRITE "assets/img/UI/uiEnemyIndicator.png"


#define ENEMY1_SPRITE "assets/img/enemies/enemyChickIdle.png"
#define ENEMY2_SPRITE "assets/img/enemies/enemyDogIdle.png"
#define ENEMY3_SPRITE "assets/img/enemies/enemyGhoulIdle.png"
#define ENEMY4_SPRITE "assets/img/enemies/enemySpiderIdle.png"


#define TAG_VULNERABLE_SPRITE "assets/img/UI/uiIconVulnerable.png"
#define TAG_RESILIENCE_SPRITE "assets/img/UI/uiIconResilience.png"

class Enemies : public Component {
    public:
        enum EnemyId {
            ENEMY1,
            ENEMY2,
            ENEMY3,
            ENEMY4
        };



        // Structure to store enemy information
        struct EnemyInfo {
            int hp;
            std::vector<Skill::SkillsTags> tags;
            std::string name;
            std::string iconPath;
        };


        Enemies(GameObject& associated, 
                EnemyId id, 
                std::vector<std::shared_ptr<Enemies>> enemiesArray);

        ~Enemies();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();
        bool Is(std::string type); 

        void CreateEnemyIndicator();
        void DeleteEnemyIndicator();
        void ApplySkillToEnemy( EnemyId enemyId);
        void ApplySkillToAllEnemies(int damage);

        void RenderTags();

        EnemyId GetId();

        // Function to initialize enemy information
        static void InitializeEnemyInfoMap();

        GameObject* enemyIndicator; 
        std::vector<std::shared_ptr<GameObject>> tags;
    private:
        EnemyId id;
        LifeBar* lifeBarEnemy;

        std::vector<std::shared_ptr<Enemies>> enemiesArray; // Used for attacks involving more then one target
        static std::map<EnemyId, EnemyInfo> enemyInfoMap;// Map to store enemy information
};
