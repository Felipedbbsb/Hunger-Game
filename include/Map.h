#pragma once


#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include "memory"
#include "GameObject.h"
#include "unordered_map"
#include "State.h"

#define MAP_SCROLL_SPEED 750

#define MAP_COLUMNS 7
#define MAP_FLOORS 15
#define MIN_COLUMN_VALUE 1
#define MAP_MAX_PATHS 4
#define MAP_SPRITE "assets/img/UI/map/uiMapBackground.png"
#define MAP_COMBAT_SPRITE "assets/img/UI/map/uiMapFight.png"

#define LINE_GAP_LENGHT  5
#define LINE_DASH_LENGHT  8

#define MAP_GRID_SIZE Vec2(1520, 3080)

#define MAP_PORC_COMBAT  0.45
#define MAP_PORC_MURAL  0.20
#define MAP_PORC_UNKNOW  0.3
#define MAP_PORC_REST  0.1

//#define MAP_PORC_COMBAT  0.0
//#define MAP_PORC_MURAL  0.0
//#define MAP_PORC_UNKNOW  0.3
//#define MAP_PORC_REST  0.0

#define LINE_DASH_LENGHT  8

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
        bool GenerateNode(std::pair<int, int> v1, std::pair<int, int> v2);
        std::map<std::vector<std::pair<int, int>>, bool> banned_edge;
        std::vector<std::vector<std::pair<int, int>>> created_edges;
        std::vector<std::pair<int, int>> created_nodes;
        static std::pair<int, int> mapPosition;
        void CreateNodeObj(std::pair<int, int> v1, NodeType type); 
        std::vector<std::pair<int, int>> GetUpperNeighbors(const std::pair<int, int>& v);
        NodeType RandomNodeType(std::pair<int, int> node, int &totalNodes, 
        bool &isMuralLastNode, int &muralCount, int &combatCount, int &restCount,int &unknwonCount);
        void CreateMap();
        int CheckCorners(int current_column, int next_column, int current_floor);
        void DrawLineMap(int x1, int y1, int x2, int y2);
        void DrawDashedLine(int x1, int y1, int x2, int y2, int dashLength, int gapLength); 
    
    private:
        
        
}; 

