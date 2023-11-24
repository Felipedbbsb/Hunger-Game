#include "Node.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Reader.h"
#include "Camera.h"
#include <algorithm> 
#include "Map.h"
#include "Mural.h"
#include "CombatState.h"
std::vector<std::pair<int, int>> Node::currentNeighbors = {};

Node::Node(GameObject &associated, NodeType type, std::pair<int, int> v1, std::vector<std::pair<int, int>> neighbors)
: Component::Component(associated),
type(type),
canVisited(false),
wasVisited(false),
floor(v1.first),
column(v1.second),
neighbors(neighbors),
ScaleNode(1)
{     
    Sprite* map_background_spr= new Sprite(associated, GetNodeSprite(type));
    associated.AddComponent((std::shared_ptr<Component>)map_background_spr);
} 
    
void Node::Start() {     

}   
  
Node::~Node(){ 
   
} 

void Node::Update(float dt){  
    if(Map::mapPosition == std::make_pair(floor, column)){
        Node::currentNeighbors = neighbors;
    }


    //if its the first choice of node
    if(Map::mapPosition.first == 0){
        if(floor == 1){
            canVisited = true;
        }
    }
    else{

        auto it = std::find(Node::currentNeighbors.begin(), Node::currentNeighbors.end(), std::make_pair(floor, column));
        if (it != Node::currentNeighbors.end()) {

            canVisited = true;
        }
        else{
            canVisited = false;
        }
    }


    Vec2 mousePos(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());
    auto& inputManager = InputManager::GetInstance();

        auto nextComponent = associated.GetComponent("Sprite");
        auto nextComponentPtr = std::dynamic_pointer_cast<Sprite>(nextComponent);
        if(nextComponentPtr){
           if (associated.box.Contains(mousePos.x- Camera::pos.x * Game::resizer, mousePos.y- Camera::pos.y * Game::resizer)){
                nextComponentPtr->SetAlpha(255);
                if(inputManager.MousePress(LEFT_MOUSE_BUTTON) && canVisited){
                    //If can be visited go to state of node
                    Map::mapPosition = std::make_pair(floor, column);
                    SetNewStage(type);
                   
                }
            }else{
                nextComponentPtr->SetAlpha(120);
            } 
            
            //if can be visited create animation expanding
            if(canVisited){
                auto scaleSprite = nextComponentPtr->GetScale();

                // Set the target scale and animation speed
                float targetScale = NODE_TIME_ANIMATION;
                float animationSpeed = NODE_TIME_ANIMATION_V;


                // Calculate the new scale based on time (dt)
                scaleSprite.x += ScaleNode * animationSpeed * dt;

                // Check if the scale has reached the minimum or maximum limit
                if (ScaleNode == 1 && scaleSprite.x >= targetScale) {
                    // Set the scale to the target value and reverse the direction
                    scaleSprite.x = targetScale;
                    ScaleNode = -1;
                } else if (ScaleNode == -1 && scaleSprite.x <= 1.0f) {
                    // Set the scale to the target value and reverse the direction
                    scaleSprite.x = 1.0f;
                    ScaleNode = 1;
                } 

                    
                // Center position original
                auto posXenterX = associated.box.x + associated.box.w / 2;
                auto posXenterY = associated.box.y + associated.box.h / 2;

                // Call SetScale with the correct number of arguments
                    nextComponentPtr->SetScale(scaleSprite.x, scaleSprite.x);


                // Postion correction
                associated.box.x = posXenterX - associated.box.w / 2;
                associated.box.y = posXenterY - associated.box.h / 2;
            }
            else{
                nextComponentPtr->SetScale(1, 1);
            }

        }    

}



void Node::Render(){
} 


void Node::SetCanVisit(bool canVisited){
    this->canVisited = canVisited;
}

void Node::SetWasVisited(bool wasVisited){
    this->wasVisited = wasVisited;
}

int Node::GetFloor(){
    return floor;
}

void Node::SetNewStage(NodeType node){
    std::vector<Enemies::EnemyId> enemiesArray = { Enemies::ENEMY1, 
                                                   Enemies::ENEMY2, 
                                                   Enemies::ENEMY3 };
    
    switch (node) {
        case NodeType::MURAL: {
            Mural* new_stage = new Mural(COMBAT_IMAGE); 
            Game::GetInstance().Push(new_stage);
            break;
        }
        case NodeType::COMBAT: {
            CombatState* new_stage = new CombatState(enemiesArray, COMBAT_IMAGE); 
            Game::GetInstance().Push(new_stage);  
            break;
        }
        case NodeType::REST: {
            Mural* new_stage = new Mural(COMBAT_IMAGE); 
            Game::GetInstance().Push(new_stage);
            break;
        }
        case NodeType::UNKNOWN: {
            Mural* new_stage = new Mural(COMBAT_IMAGE); 
            Game::GetInstance().Push(new_stage);
            break;
        }
        case NodeType::BOSS: {
            Mural* new_stage = new Mural(COMBAT_IMAGE); 
            Game::GetInstance().Push(new_stage);
            break;
        }
    }
}

std::string Node::GetNodeSprite(NodeType node){
    std::string spriteNode;
        switch (node) {
            case NodeType::MURAL: spriteNode = MAP_MURAL_SPRITE; break;
            case NodeType::COMBAT: spriteNode = MAP_COMBAT_SPRITE; break;
            case NodeType::REST: spriteNode = MAP_REST_SPRITE; break;
            case NodeType::UNKNOWN: spriteNode = MAP_UNKNOWN_SPRITE; break;
            case NodeType::BOSS: spriteNode = MAP_BOSS_SPRITE; break;
        }
    return spriteNode ;   
}

bool Node::Is(std::string type){
    return (type == "Node");
}

