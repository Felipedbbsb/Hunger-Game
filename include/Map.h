#pragma once


#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include "memory"
#include "GameObject.h"
#include "unordered_map"
#include "State.h"

#define MAP_COLUMNS 5
#define MAP_FLOORS 15
#define MIN_COLUMN_VALUE 1
#define MAP_MAX_PATHS 4
#define MAP_SPRITE "assets/img/UI/map/uiMapBackground.png"
#define MAP_COMBAT_SPRITE "assets/img/UI/map/uiMapFight.png"

#define MAP_GRID_SIZE Vec2(1820, 2000)

// Node possible types
enum NodeType { MURAL, COMBAT, REST, UNKNOWN, BOSS };

// Node Struct
typedef struct NodeStats {
    int x;
    int y;
    NodeType type;
    std::weak_ptr<GameObject> gameObject = {};
} NodeStats;


class Map : public State{
    public:
        Map();
        ~Map();
        void Start();
        void Render();
        void Update(float dt);
        void Pause();
        void Resume();
        void LoadAssets();
        static bool GenerateNode(std::pair<int, int> v1, std::pair<int, int> v2);
        static std::map<std::vector<std::pair<int, int>>, bool> banned_edge;
        static std::vector<std::vector<std::pair<int, int>>> created_edges;
        static std::vector<std::pair<int, int>> created_nodes;
        static std::pair<int, int> mapPosition;
        void CreateNodeObj(std::pair<int, int> v1); 
        std::vector<std::pair<int, int>> GetUpperNeighbors(const std::pair<int, int>& v);
    
    private:
        void CreateMap();
        int CheckCorners(int current_column, int next_column, int current_floor);
        void DrawLineMap(int x1, int y1, int x2, int y2);
         
        //std::vector<MapPath> map;

        
}; 

