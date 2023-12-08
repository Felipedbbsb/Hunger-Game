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
#include "Enemies.h"

std::vector<std::pair<int, int>> Node::currentNeighbors = {};

Node::Node(GameObject &associated, NodeType type, std::pair<int, int> v1, std::vector<std::pair<int, int>> neighbors)
: Component::Component(associated),
type(type),
canVisited(false),
wasVisited(false),
floor(v1.first),
column(v1.second),
neighbors(neighbors),
ScaleNode(1),
iconVisited(nullptr)
{     
    Sprite* map_background_spr= new Sprite(associated, GetNodeSprite(type));
    associated.AddComponent((std::shared_ptr<Component>)map_background_spr);
} 
    
void Node::Start() {     

}   
  
Node::~Node(){ 
   if(iconVisited != nullptr){
        iconVisited->RequestDelete();
        iconVisited = nullptr;
    }
} 

void Node::Update(float dt){  
    if(wasVisited){
        if(iconVisited == nullptr){
            CreateIconVisited();
        }
        


    }
    else{
        if(iconVisited != nullptr){
            iconVisited->RequestDelete();
            iconVisited = nullptr;
        }
    }


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

    wasVisited = true;


    switch (node) {

        case NodeType::MURAL: {
            CreateMural();
            break;
        }
        case NodeType::COMBAT: {
            CreateCombat();  
            break;
        }
        case NodeType::REST: {
            Mural* new_stage = new Mural(COMBAT_IMAGE); 
            Game::GetInstance().Push(new_stage);
            break;
        } 
        case NodeType::UNKNOWN: {
            int randomizedEncounter = rand() % 3 + 1;
            //COMBAT
            if(randomizedEncounter == 1){
                CreateCombat(); 

            }
 
            //MURAL
            else if(randomizedEncounter == 2){
                CreateMural();
            }

            //RANDOM ENCOUNTER
            else if(randomizedEncounter == 3){
                CreateMural();
            }

           break;
        }
        case NodeType::BOSS: {
            CreateCombat();  
            break;
        }
    }
}

void Node::CreateIconVisited(){
    //first set the size from the node to 1
    auto nextComponent = associated.GetComponent("Sprite");
        auto nextComponentPtr = std::dynamic_pointer_cast<Sprite>(nextComponent);
        if(nextComponentPtr){
            nextComponentPtr->SetScale(1,1);
        }


    iconVisited = new GameObject(associated.box.x, associated.box.y);
    Sprite* iconVisited_spr = new Sprite(*iconVisited, MAP_VISITED_SPRITE);
    iconVisited->AddComponent(std::shared_ptr<Sprite>(iconVisited_spr));

    iconVisited->box.x += associated.box.w / 2 - iconVisited->box.w / 2;
    iconVisited->box.y += associated.box.h / 2 - iconVisited->box.h / 2;

    Game::GetInstance().GetCurrentState().AddObject(iconVisited);
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

void Node::CreateMural(){
    int randomValue = std::rand() % 2 + 1;

    // Append the random value to the sprite path
    std::string spriteBg = MAP_MURAL_BACKGROUND;
    spriteBg.insert(spriteBg.find_last_of('.'), std::to_string(randomValue));

    Mural* new_stage = new Mural(spriteBg); 
    Game::GetInstance().Push(new_stage);  
}

void Node::CreateCombat(){
    std::vector<Enemies::EnemyId> enemiesArray = GetRandomEncounter(Map::mapPosition.first - 1);                                               
    

    CombatState* new_stage = new CombatState(enemiesArray, COMBAT_IMAGE); 
    Game::GetInstance().Push(new_stage);  
}

std::vector<Enemies::EnemyId> Node::GetRandomEncounter(int floorPostion){
    
    std::map< int, std::vector<Enemies::EnemyId>> encounterMap;

    /*
    //encounters from 1-5
    encounterMap[1] = { Enemies::CultistGreen, Enemies::CultistGreen};
    encounterMap[2] = { Enemies::CultistGreen, Enemies::CultistRed };
    encounterMap[3] = { Enemies::Frog, Enemies::CultistGreen };
    encounterMap[4] = { Enemies::Frog, Enemies::Radog};
    encounterMap[5] = { Enemies::Frog, Enemies::Radog, Enemies::CultistGreen };

    //encounters from 6-10
    encounterMap[6] = { Enemies::Radog, Enemies::Cat };
    encounterMap[7] = { Enemies::CultistRed, Enemies::Cat };
    encounterMap[8] = { Enemies::CultistGreen, Enemies::CultistGreen, Enemies::Cat };
    encounterMap[9] = { Enemies::Cat, Enemies::Cat};
    encounterMap[10] = { Enemies::CultistGreen, Enemies::CultistRed, Enemies::Cat };
    
    //encounters from 11-15
    encounterMap[11] = { Enemies::Parakeet, Enemies::Cat };
    encounterMap[12] = { Enemies::CultistRed, Enemies::Cat };
    encounterMap[13] = { Enemies::CultistGreen, Enemies::CultistGreen, Enemies::Cat };
    encounterMap[14] = { Enemies::Cat, Enemies::Cat};
    encounterMap[15] = { Enemies::CultistGreen, Enemies::CultistRed, Enemies::Cat };
    */

   //Test release version 70%
    //encounters from 1-5
    encounterMap[1] = { Enemies::CultistGreen, Enemies::CultistGreen};
    encounterMap[2] = { Enemies::CultistGreen, Enemies::CultistGreen };
    encounterMap[3] = { Enemies::CultistGreen, Enemies::CultistGreen };
    encounterMap[4] = { Enemies::CultistGreen, Enemies::CultistGreen};
    encounterMap[5] = { Enemies::CultistGreen, Enemies::CultistRed, Enemies::CultistRed };

    //encounters from 6-10
    encounterMap[6] = { Enemies::CultistRed, Enemies::Parakeet};
    encounterMap[7] = { Enemies::CultistRed, Enemies::CultistRed };
    encounterMap[8] = { Enemies::CultistRed, Enemies::CultistGreen, Enemies::CultistRed };
    encounterMap[9] = { Enemies::CultistRed, Enemies::Parakeet};
    encounterMap[10] = { Enemies::CultistRed, Enemies::CultistRed, Enemies::CultistRed };
    
    //encounters from 11-15
    encounterMap[11] = { Enemies::CultistPurple, Enemies::Parakeet };
    encounterMap[12] = { Enemies::CultistRed, Enemies::CultistPurple };
    encounterMap[13] = { Enemies::CultistGreen, Enemies::CultistGreen, Enemies::CultistPurple };
    encounterMap[14] = { Enemies::CultistPurple, Enemies::CultistPurple};
    encounterMap[15] = { Enemies::CultistGreen, Enemies::CultistRed, Enemies::CultistPurple };


    int randomValue = std::rand() % 5 + 1;

    //encounters from 1-5
    if(floorPostion < 6){
        
        return encounterMap[randomValue];
    }
    //encounters from 6-10
    else if(floorPostion < 11){
        return encounterMap[randomValue + 5];
    }
    //encounters from 11-15
    else if(floorPostion < 16){
        return encounterMap[randomValue + 10];
    }
    //encounter BOSS
    else if(floorPostion == 16){
        return encounterMap[randomValue + 10];
    }

    return encounterMap[1];
}

bool Node::Is(std::string type){
    return (type == "Node");
}

 