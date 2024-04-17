#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"
#include "Map.h"
#include "Enemies.h"
#include "Music.h"

#define NODE_TIME_ANIMATION 1.5f
#define NODE_TIME_ANIMATION_V 0.5

#define MAP_MURAL_SPRITE "assets/img/UI/map/uiMapMural.png"
#define MAP_COMBAT_SPRITE "assets/img/UI/map/uiMapFight.png"
#define MAP_REST_SPRITE "assets/img/UI/map/uiMapRest.png"
#define MAP_UNKNOWN_SPRITE "assets/img/UI/map/uiMapMistery.png"
#define MAP_BOSS_SPRITE "assets/img/UI/map/uiMapBoss.png"
#define MAP_VISITED_SPRITE "assets/img/UI/map/uiMapVisited.png"

#define MAP_MURAL_BACKGROUND "assets/img/UI/mural/Mural.png"

class Node : public Component {
    public:
        Node(GameObject &associated, NodeType type, std::pair<int, int> v1, std::vector<std::pair<int, int>> neighbors, bool visited);
        ~Node();
        void Update(float dt);
        void Render();
        void Start();
        void SetCanVisit(bool canVisited);
        void SetWasVisited(bool wasVisited);
        int GetFloor();
        std::string GetNodeSprite(NodeType node);
        bool Is(std::string type);
        static std::vector<std::pair<int, int>> currentNeighbors;
        void SetNewStage(NodeType node);
        void CreateIconVisited();
    private:
        void CreateRest();
        void CreateMural();
        void CreateCombat();
            std::vector<Enemies::EnemyId> GetRandomEncounter(int floorPostion);

        NodeType type;
        bool canVisited;
        bool wasVisited;
        int floor;
        int column;
        std::vector<std::pair<int, int>> neighbors;
        std::pair<int, int> _v1;
        int ScaleNode;
        GameObject* iconVisited;
        GameObject* selectSFX; 
}; 