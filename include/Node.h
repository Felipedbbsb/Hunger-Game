#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"
#include "Map.h"

#define NODE_TIME_ANIMATION 1.2f
#define NODE_TIME_ANIMATION_V 0.375

#define MAP_MURAL_SPRITE "assets/img/UI/map/uiMapMural.png"
#define MAP_COMBAT_SPRITE "assets/img/UI/map/uiMapFight.png"
#define MAP_REST_SPRITE "assets/img/UI/map/uiMapFight.png"
#define MAP_UNKNOWN_SPRITE "assets/img/UI/map/uiMapMistery.png"
#define MAP_BOSS_SPRITE "assets/img/UI/map/uiMapBoss.png"

class Node : public Component {
    public:
        Node(GameObject &associated, NodeType type, std::pair<int, int> v1, std::vector<std::pair<int, int>> neighbors);
        ~Node();
        void Update(float dt);
        void Render();
        void Start();
        void SetCanVisit(bool canVisited);
        void SetWasVisited(bool wasVisited);
        int GetFloor();
        std::string GetNodeSprite(NodeType node);
        bool Is(std::string type);


    private:
        bool canVisited;
        bool wasVisited;
        int floor;
        int column;
        std::vector<std::pair<int, int>> neighbors;
        int ScaleNode;
};