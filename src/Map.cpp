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
#include "VictoryState.h" 
#include "EndState.h" 

std::pair<int, int> Map::mapPosition = std::make_pair(0, 0); 

Map::Map() : State::State(){  
    Map::mapPosition = std::make_pair(0, 0); 
    CreateMap();

    

}
  
Map::~Map() { 
    Map::mapPosition = std::make_pair(0, 0); 
}

void Map::Update(float dt) {
    if(Map::mapPosition.first == MAP_FLOORS + 1){
        EndState* newState = new EndState();    
        Game::GetInstance().Push(newState);  
        popRequested = true;
    }

    InputManager& input = InputManager::GetInstance();
 
    

    if (input.KeyPress(ESCAPE_KEY) || input.QuitRequested()) {
        quitRequested = true;
    }

     if ((input.IsKeyDown(W_KEY) || input.IsKeyDown(UP_ARROW_KEY)) && Camera::pos.y < MAP_GRID_SIZE.y - RESOLUTION_HEIGHT) {
        Camera::pos.y += MAP_SCROLL_SPEED * dt;
    }

    // Se a tecla S ou a seta para baixo for pressionada e a posição da câmera não ultrapassar o limite inferior
    if ((input.IsKeyDown(S_KEY) || input.IsKeyDown(DOWN_ARROW_KEY)) && Camera::pos.y > 0) {
        Camera::pos.y -= MAP_SCROLL_SPEED * dt;
    }

    // Log scroll values
    //std::cout << input.isMouseWheelScrolled()<<"  ScrollX: " << input.GetScrollX() << ", ScrollY: " << input.GetScrollY() << std::endl;


    // Obtenha os valores atuais de scroll
    //int scrollX = input.GetScrollX();
    //int scrollY = input.GetScrollY();
 
    // Mova a câmera com base no scroll
    //if(input.isMouseWheelScrolled()){
    //    Camera::pos.y += scrollY * MAP_SCROLL_SPEED * dt;
    //}
    

    UpdateArray(dt); 
    Camera::Update(dt);

    State::Update(dt);
} 

void Map::LoadAssets() {
    //background
    GameObject *map_background = new GameObject(0,0);
    Sprite* map_background_spr= new Sprite(*map_background, MAP_SPRITE);
    map_background->AddComponent((std::shared_ptr<Component>)map_background_spr);

    map_background->box.x = RESOLUTION_WIDTH  / 2 - map_background->box.w / 2;
    map_background->box.y = RESOLUTION_HEIGHT - map_background->box.h + MAP_GRID_SIZE.y / (MAP_FLOORS + 2);

    //CameraParallax *map_background_cmfl = new CameraParallax(*map_background, 1);
    //map_background->AddComponent((std::shared_ptr<CameraParallax>)map_background_cmfl);

    AddObject(map_background);


    int totalNodes = created_nodes.size();
    bool isMuralLastNode = false;
    int muralCount = totalNodes * MAP_PORC_MURAL;
    int combatCount = totalNodes * MAP_PORC_COMBAT;
    int restCount = totalNodes * MAP_PORC_COMBAT;
    int unknwonCount = totalNodes * MAP_PORC_REST;
    for (const auto& node : created_nodes) {
        CreateNodeObj(node, RandomNodeType(node, totalNodes, isMuralLastNode, muralCount, combatCount, restCount, unknwonCount));
    }


    //====================================BOSS===============================
    GameObject *new_node = new GameObject();
    Node* node_spr = new Node(*new_node, NodeType::BOSS, std::make_pair(MAP_FLOORS + 1, 1), {});
    new_node->AddComponent((std::shared_ptr<Component>)node_spr);

    new_node->box.x = RESOLUTION_WIDTH / 2; //Centralized

    new_node->box.y = RESOLUTION_HEIGHT - (MAP_FLOORS + 1) * MAP_GRID_SIZE.y / (MAP_FLOORS + 2); //plus 2 for offset and BOSS

    //Centralize in center of spirte
    new_node->box.x -= new_node->box.w/2;
    new_node->box.y -= new_node->box.h/2;
  
    AddObject(new_node);


    Music noncombatMusic;
    noncombatMusic.Open("assets/audio/songNonCombat.mp3");
    noncombatMusic.Play();     


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

    new_node->box.y = RESOLUTION_HEIGHT - v1.first * MAP_GRID_SIZE.y / (MAP_FLOORS + 2); //Centralized

    //Centralize in center of spirte
    new_node->box.x -= new_node->box.w/2;
    new_node->box.y -= new_node->box.h/2;
 
    AddObject(new_node);
}

NodeType Map::RandomNodeType(std::pair<int, int> node, int &totalNodes, bool &isMuralLastNode, int &muralCount, int &combatCount, int &restCount,int &unknwonCount) {
    // Gerar um número entre 1 e 100 (inclusive)
    int randomPercent = std::rand() % totalNodes + 1;


    --totalNodes;

    // O primeiro andar sempre é um combate
    // O 15º andar sempre é um mural
    // O 14º andar nunca é um mural.
    // Não podem existir salas de mural consecutivas.

    //floor exceptions
    if(node.first == MAP_FLOORS){
        --restCount;
        isMuralLastNode = false;
        return NodeType::REST;
    }
    else if(node.first == 1){
        --combatCount;
        isMuralLastNode = false;
        return NodeType::COMBAT; 
    }


    if (randomPercent <= muralCount && !isMuralLastNode) {
        --muralCount;
        isMuralLastNode = true; 
        return NodeType::MURAL;
    } 
    else if (randomPercent <= muralCount + combatCount) {
        --combatCount;
        isMuralLastNode = false;
        return NodeType::COMBAT; 
    } 
    else if(randomPercent <= muralCount + combatCount + unknwonCount) {
        --unknwonCount;
        isMuralLastNode = false;
        return NodeType::UNKNOWN;  
    }
    else if(randomPercent <= muralCount + combatCount + unknwonCount + restCount && node.first != MAP_FLOORS - 1 && !isMuralLastNode) {
        --restCount;
        isMuralLastNode = true;
        return NodeType::REST;  
    }
    
    //exception
    return NodeType::UNKNOWN;
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

    if(v.first == MAP_FLOORS){
        upperNeighbors.push_back(std::make_pair(MAP_FLOORS + 1, 1));
    }


    return upperNeighbors;
}

void Map::Render() {      
    RenderArray();


    // Render created lines
    for (const auto& edge : created_edges) {
        for (size_t i = 1; i < edge.size(); ++i) {
            // Obter as coordenadas dos pontos da aresta
            if(edge[i].first <= MAP_FLOORS){
                int x1 = edge[i - 1].second * MAP_GRID_SIZE.x / (MAP_COLUMNS + 1) +
                         RESOLUTION_WIDTH / 2 - MAP_GRID_SIZE.x / 2 + Camera::pos.x;
                int y1 = RESOLUTION_HEIGHT - edge[i - 1].first * MAP_GRID_SIZE.y / (MAP_FLOORS + 2) + Camera::pos.y; 

                int x2 = edge[i].second * MAP_GRID_SIZE.x / (MAP_COLUMNS + 1) +
                        RESOLUTION_WIDTH  / 2 - MAP_GRID_SIZE.x / 2 + Camera::pos.x;
                int y2 = RESOLUTION_HEIGHT - edge[i].first * MAP_GRID_SIZE.y / (MAP_FLOORS + 2) + Camera::pos.y; //plus 2 for offset and BOSS

                // Desenhar a linha
                DrawDashedLine(x1, y1, x2, y2, LINE_DASH_LENGHT, LINE_GAP_LENGHT);  

                //Boss
                if(edge[i].first == MAP_FLOORS){
                    x1 = edge[i].second * MAP_GRID_SIZE.x / (MAP_COLUMNS + 1) +
                    RESOLUTION_WIDTH / 2 - MAP_GRID_SIZE.x / 2 + Camera::pos.x;
                    y1 = RESOLUTION_HEIGHT - edge[i].first * MAP_GRID_SIZE.y / (MAP_FLOORS + 2) + Camera::pos.y; 

                    x2 = RESOLUTION_WIDTH / 2 + Camera::pos.x;
                    y2 = RESOLUTION_HEIGHT - (MAP_FLOORS + 1) * MAP_GRID_SIZE.y / (MAP_FLOORS + 2) + Camera::pos.y; //plus 2 for offset and BOSS

                    // Desenhar a linha
                    DrawDashedLine(x1, y1, x2, y2, LINE_DASH_LENGHT, LINE_GAP_LENGHT);  

                }

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

    Camera::Unfollow();
    Camera::pos.x = 0;


    Camera::pos.y = mapPosition.first * MAP_GRID_SIZE.y / (MAP_FLOORS + 2) - RESOLUTION_HEIGHT/2;
    if(Camera::pos.y  < 0){
        Camera::pos.y = 0;
    }

}

 

 
void Map::DrawLineMap(int x1, int y1, int x2, int y2) {
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black color (R, G, B, A)

    // Draw a thicker line by drawing multiple connected lines
    int thickness = 5;  // Set the thickness of the line
    for (int i = 0; i < thickness; ++i) {
        SDL_RenderDrawLine(renderer, x1, y1 + i, x2, y2 + i);
    }

    // Restore the original color (optional, depending on the context)
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color (R, G, B, A)
}

// Function to draw a dashed line between two points
void Map::DrawDashedLine(int x1, int y1, int x2, int y2, int dashLength, int gapLength) {
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black color (R, G, B, A)

    // Calculate the length and direction of the line
    float length = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    float dx = (x2 - x1) / length;
    float dy = (y2 - y1) / length;

    // Draw the dashed line
    float currentLength = 0;
    bool drawDash = true;
    while (currentLength <= length) {
        if (drawDash) {
            SDL_RenderDrawLine(renderer, x1 + currentLength * dx, y1 + currentLength * dy,
                               x1 + fmin(currentLength + dashLength, length) * dx,
                               y1 + fmin(currentLength + dashLength, length) * dy);
        }

        currentLength += dashLength + gapLength;
        drawDash = !drawDash;
    }

    // Restore the original color (optional, depending on the context)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color (R, G, B, A)
}