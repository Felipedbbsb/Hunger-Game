#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "GameObject.h"
#include "Component.h"
#include "Sprite.h"

#define ENEMY1_SPRITE "assets/img/enemies/enemyChickIdle.png"
#define ENEMY2_SPRITE "assets/img/enemies/enemyDogIdle.png"
#define ENEMY3_SPRITE "assets/img/enemies/enemyGhoulIdle.png"
#define ENEMY4_SPRITE "assets/img/enemies/enemySpiderIdle.png"

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
        std::vector<std::string> tags;
        std::string name;
        std::string iconPath;
    };


    Enemies(GameObject& associated, EnemyId id);
    ~Enemies();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();
    bool Is(std::string type);

    EnemyId GetId();

    // Function to initialize enemy information
    static void InitializeEnemyInfoMap();

private:
    EnemyId id;

    // Map to store enemy information
    static std::map<EnemyId, EnemyInfo> enemyInfoMap;
};
