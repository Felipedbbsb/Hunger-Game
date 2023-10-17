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
#include "Rect.h"

#define ENEMY_INDICATOR_SPRITE "assets/img/UI/uiEnemyIndicator.png"


#define ENEMY1_SPRITE "assets/img/enemies/enemyChickIdle.png"
#define ENEMY2_SPRITE "assets/img/enemies/enemyDogIdle.png"
#define ENEMY3_SPRITE "assets/img/enemies/enemyGhoulIdle.png"
#define ENEMY4_SPRITE "assets/img/enemies/enemySpiderIdle.png"


#define TAGS_SPACING 35.0f


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
                EnemyId id);
 
        ~Enemies();
        void Update(float dt);
        void Render();
        void Start();
        void Pause(); 
        void Resume();
        bool Is(std::string type); 

        void CreateEnemyIndicator();
        void DeleteEnemyIndicator();
        void ApplySkillToEnemy();
        void ApplySkillToSingleEnemy(  Skill::SkillInfo& skillInfo);
        void ApplySkillToAllEnemies();
        void ApplyTags(std::vector<Skill::SkillsTags> skillTags);
        std::weak_ptr<GameObject>  AddObjTag(Skill::SkillsTags tag);

        EnemyId GetId(); 

        // Function to initialize enemy information
        static void InitializeEnemyInfoMap();


        GameObject* enemyIndicator;  

        static std::vector<std::shared_ptr<Enemies>> enemiesArray;
        static int enemiesCount;
        static int SkillAllenemies;
    
    private:
        EnemyId id;
        LifeBar* lifeBarEnemy;
        Rect enemyHitbox;

        int tagSpaceCount;

        //Enemie stats
        int hp;
        std::vector<Skill::SkillsTags> tags;
        std::string name;
        std::string iconPath;

        std::vector<std::weak_ptr<GameObject>> enemytags;//only used for destructor
        
        std::map<Skill::SkillsTags, int> tagCountMap; // Map to track tag counts, separated from enemytags
 
         // Used for attacks involving more then one target
        static std::map<EnemyId, EnemyInfo> enemyInfoMap;// Map to store enemy information

};
