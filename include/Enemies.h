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
#include "Timer.h"

#define ENEMY_INDICATOR_SPRITE "assets/img/UI/uiEnemyIndicator.png"

#define ENEMY_INTENTON_SPRITE "assets/img/UI/uiIntentionEnemy.png"

#define ENEMY1_SPRITE "assets/img/enemies/enemyChickIdle.png"
#define ENEMY2_SPRITE "assets/img/enemies/enemyDogIdle.png"
#define ENEMY3_SPRITE "assets/img/enemies/enemyGhoulIdle.png"
#define ENEMY4_SPRITE "assets/img/enemies/enemySpiderIdle.png"


#define INTENTION_COOLDOWN 1.5

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
            std::vector<Tag::Tags> tags;
            std::string name;
            std::string iconPath;
            std::vector<Skill::SkillId> skills;
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

        void SetupInteractionScreen(Skill::AttackType attackType, Skill::TargetType whoAttacks);

        void CreateEnemyIndicator();
        void DeleteEnemyIndicator();
        
        void ApplySkillToEnemy();
        void ApplySkillToSingleEnemy(int damage, std::vector<Tag::Tags> tags);
        void ApplySkillToAllEnemies();
        void ApplyTags(std::vector<Tag::Tags> skillTags);
        std::weak_ptr<GameObject>  AddObjTag(Tag::Tags tag);
        bool HasTag(Tag::Tags tagToCheck);
        bool HasTagPlayer(Tag::Tags tagToCheck, std::vector<Tag::Tags> tags);
        void ActivateTag(Tag::Tags tag); 

        EnemyId GetId(); 

        // Function to initialize enemy information
        static void InitializeEnemyInfoMap();


        GameObject* enemyIndicator;  
        GameObject* intention; 

        static std::vector<std::shared_ptr<Enemies>> enemiesArray;
        static int enemiesCount;
        static int SkillAllenemies;
        static int provokedEnemies;
        static int enemiesToAttack;
        static bool enemyAttacking;

        // Used for attacks involving more then one target
        static std::map<EnemyId, EnemyInfo> enemyInfoMap;// Map to store enemy information
        
    
    private:
        EnemyId id;
        LifeBar* lifeBarEnemy;
        Rect enemyHitbox;

        int tagSpaceCount;

        //Enemie stats
        int hp;
        std::vector<Tag::Tags> tags;
        std::string name;
        std::string iconPath;
        std::vector<Skill::SkillId> skills;

        bool thisEnemyAttacked;

        std::vector<std::weak_ptr<GameObject>> enemytags;//only used for destructor
        
        std::map<Tag::Tags, int> tagCountMap; // Map to track tag counts, separated from enemytags
 
        

        Timer intentionTimer;

        void CreateIntention();
        void DeleteIntention();

};
