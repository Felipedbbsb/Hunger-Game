#include "Map.h"
#include "Game.h"
#include "InputManager.h" 
#include "CameraFollower.h"
#include "Camera.h"
#include "Text.h"
#include "UI.h"
#include "Mother.h" 
#include "Daughter.h"
#include "Enemies.h" 
#include "Skill.h" 
#include "Papiro.h" 
#include "SkillSelection.h" 
#include "CameraParallax.h"
#include "Protected.h"
#include "Mural.h"
#include "Node.h"
#include "SDL_include.h" 

std::map<std::vector<std::pair<int, int>>, bool> Map::banned_edge;
std::vector<std::vector<std::pair<int, int>>> Map::created_edges;
std::vector<std::pair<int, int>> Map::created_nodes;
std::pair<int, int> Map::mapPosition = std::make_pair(0, 0); 

Map::Map() : State::State(){ 
    CreateMap();

}
  
Map::~Map() {}

void Map::Update(float dt) {
    InputManager& input = InputManager::GetInstance();
 
    // If the event is quit, set the termination flag
    if (input.KeyPress(ESCAPE_KEY) || input.QuitRequested()) {
        quitRequested = true;
    }

    UpdateArray(dt); 
    Camera::Update(dt);

    State::Update(dt);
} 

void Map::LoadAssets() {
    //background
    GameObject *map_background = new GameObject(-1000,-1000);
    Sprite* map_background_spr= new Sprite(*map_background, MAP_SPRITE);
    map_background->AddComponent((std::shared_ptr<Component>)map_background_spr);

    //map_background->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - firstPathObj->box.w / 2;

    CameraParallax *map_background_cmfl = new CameraParallax(*map_background, 1);
    map_background->AddComponent((std::shared_ptr<CameraParallax>)map_background_cmfl);

    AddObject(map_background);


    int totalNodes =created_nodes.size();
    int muralCount = 0;
    for (const auto& node : created_nodes) {
        CreateNodeObj(node, RandomNodeType(totalNodes, muralCount));
    }
}

void Map::CreateMap(){
    for(int i=1; i<=MAP_MAX_PATHS; i++){
        int floors_made = 0;
        int current_column = 0;
        int upperFloor, nodeCreated, new_column;
        while(floors_made < MAP_FLOORS){

            //first floor 
            if(floors_made == 0){
                int auxColumn = rand() % MAP_COLUMNS + 1; //can choose any column to start
                upperFloor = rand() % 3 - 1; //chooses between left, middle or right
                upperFloor = CheckCorners(auxColumn, upperFloor, floors_made);

                nodeCreated = GenerateNode(std::make_pair(floors_made + 1, auxColumn), std::make_pair(floors_made + 2, auxColumn + upperFloor));
                new_column = auxColumn + upperFloor;
            }
            else{
                upperFloor = rand() % 3 - 1; //chooses between left, middle or right
                upperFloor = CheckCorners(current_column, upperFloor, floors_made);
                nodeCreated = GenerateNode(std::make_pair(floors_made + 1, current_column), std::make_pair( floors_made + 2, current_column + upperFloor));
                new_column = current_column + upperFloor;
            }

            //was possible to create node
            if(nodeCreated){
                current_column = new_column; //normalize current_column
                floors_made++;
            }

        }
    } 
}

int Map::CheckCorners(int current_column, int next_column, int current_floor) {     
    if(current_column == 1){ 
        if(next_column == -1){ // if column first cannot go left
            return rand() % 2;
        }
    }
    else if(current_column == MAP_COLUMNS){
        if(next_column == 1){ // if column first cannot go right
            return rand() % 2 - 1;
        }
    }
    return next_column; // no changes
}

bool Map::GenerateNode(std::pair<int, int> v1, std::pair<int, int> v2){
    
    // v = {floor number, column number}


    // Check if the edge already exists in created_edges
    if (std::find_if(created_edges.begin(), created_edges.end(),
        [v1, v2](const std::vector<std::pair<int, int>>& edge) {
            return edge[0] == v1 && edge[1] == v2;
        }) != created_edges.end()) {
        return true; // Edge already exists, consider as created to continue
    }
    
    //if(v1.first > v2.first) {std::swap(v1, v2);}  // guarantees that v1 will always be the bottom floor and v2 the top floor
    std::vector<std::pair<int, int>> vaux = {v1, v2};
    if(Map::banned_edge[vaux]) {
        return false;
    }  // you cannot create this edge 

     
    // you can create this edge 
    else{
        for(int i=v2.second - 1; i>=1; i--){
            for(int j=v1.second + 1; j<=MAP_COLUMNS; j++){
                std::vector<std::pair<int, int>> vaux2 = {std::make_pair(v1.first, j), std::make_pair(v1.first, i)};
                Map::banned_edge[vaux2] = true;
            }
        }

        for(int i=v1.second - 1; i>=1; i--){
            for(int j=v2.second + 1; j<=MAP_COLUMNS; j++){
                std::vector<std::pair<int, int>> vaux2 = {std::make_pair(v1.first, i), std::make_pair(v2.first, j)};
                Map::banned_edge[vaux2] = true;
            }
        } 

        
        created_edges.push_back({v1, v2});//create edge
        
        //create node
        // Check if the node already exists in created_nodes
        if (!(std::find(created_nodes.begin(), created_nodes.end(), v1) != created_nodes.end())) {
            created_nodes.push_back(v1); //pushes to array 
        }
        
        return true;
    }
}

void Map::CreateNodeObj(std::pair<int, int> v1, NodeType type) {     
    GameObject *new_node = new GameObject();
    Node* node_spr = new Node(*new_node, type, v1, GetUpperNeighbors(v1));
    new_node->AddComponent((std::shared_ptr<Component>)node_spr);

    new_node->box.x = RESOLUTION_WIDTH / 2 - MAP_GRID_SIZE.x / 2; //Centralized
    new_node->box.x += v1.second * MAP_GRID_SIZE.x / (MAP_COLUMNS + 1);

    new_node->box.y = RESOLUTION_HEIGHT - v1.first * MAP_GRID_SIZE.y / (MAP_COLUMNS + 1); //Centralized

    //Centralize in center of spirte
    new_node->box.x -= new_node->box.w/2;
    new_node->box.y -= new_node->box.h/2;

    AddObject(new_node);
}

NodeType Map::RandomNodeType(int totalNodes, int& muralCount) {
    // Gerar um número entre 1 e 100 (inclusive)
    int randomPercent = std::rand() % 100 + 1;

    // Verificar se deve ser um mural
    if (randomPercent <= 20 && muralCount < totalNodes * 0.2) {
        ++muralCount;
        return NodeType::MURAL;
    } else if (randomPercent <= 65) {
        return NodeType::COMBAT; 
    } else {
        return NodeType::UNKNOWN;  // Substitua "UNKNOWN" pelo tipo correto de sala
    }
}

std::vector<std::pair<int, int>> Map::GetUpperNeighbors(const std::pair<int, int>& v) {
    std::vector<std::pair<int, int>> upperNeighbors;

    // Iterar sobre as arestas criadas
    for (const auto& edge : created_edges) {
        // Iterar sobre os pontos da aresta
        for (size_t i = 0; i < edge.size(); ++i) {
            // Verificar se o ponto atual é o mesmo que v
            if (edge[i] == v) {

                // Adicionar o próximo ponto (vizinho de cima) ao vetor de vizinhos
                if (i < edge.size() - 1) {
                    upperNeighbors.push_back(edge[i + 1]);
                }
            }
        }
    }

    // Remover duplicatas no vetor de vizinhos
    upperNeighbors.erase(std::unique(upperNeighbors.begin(), upperNeighbors.end()), upperNeighbors.end());


    return upperNeighbors;
}

void Map::Render() {      
    RenderArray();


    // Renderizar as linhas criadas
    for (const auto& edge : created_edges) {
        for (size_t i = 1; i < edge.size(); ++i) {
            // Obter as coordenadas dos pontos da aresta
            if(edge[i].first <= MAP_FLOORS){
                int x1 = edge[i - 1].second * MAP_GRID_SIZE.x / (MAP_COLUMNS + 1) +
                         RESOLUTION_WIDTH / 2 - MAP_GRID_SIZE.x / 2 + Camera::pos.x;
                int y1 = RESOLUTION_HEIGHT - edge[i - 1].first * MAP_GRID_SIZE.y / (MAP_COLUMNS + 1) + Camera::pos.y;

                int x2 = edge[i].second * MAP_GRID_SIZE.x / (MAP_COLUMNS + 1) +
                        RESOLUTION_WIDTH  / 2 - MAP_GRID_SIZE.x / 2 + Camera::pos.x;
                int y2 = RESOLUTION_HEIGHT - edge[i].first * MAP_GRID_SIZE.y / (MAP_COLUMNS + 1) + Camera::pos.y;

                // Desenhar a linha
                DrawLineMap(x1, y1, x2, y2);  
            }
            
        }  
    }
 

    State::Render();
}


void Map::Start() {
    LoadAssets();
    StartArray();
    started = true;  
}
 
void Map::Pause() {
    State::Pause();
}

void Map::Resume() {
    State::Resume();

    GameObject* focusCamera =  new GameObject(0, 0);
    Camera::Follow(focusCamera);
    Camera::pos.x = 0;
    Camera::pos.y = 0;
}

 

 
void Map::DrawLineMap(int x1, int y1, int x2, int y2) {
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Cor preta (R, G, B, A)
    
    // Desenhar a linha
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    
    // Restaurar a cor original (opcional, dependendo do contexto)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Cor branca (R, G, B, A)
}  