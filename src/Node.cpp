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
#include "Rest.h"

std::vector<std::pair<int, int>> Node::currentNeighbors = {};

Node::Node(GameObject &associated, NodeType type, std::pair<int, int> v1, std::vector<std::pair<int, int>> neighbors, bool visited)
: Component::Component(associated),
type(type),
canVisited(false),
wasVisited(visited),
floor(v1.first),
column(v1.second),
neighbors(neighbors),
_v1(v1),
ScaleNode(1),
iconVisited(nullptr),
selectSFX(nullptr)
{     
    new Sprite(associated, GetNodeSprite(type));

} 
    
void Node::Start() {     

}   
  
Node::~Node(){ 
   if(iconVisited != nullptr){
        iconVisited->RequestDelete();
        iconVisited = nullptr;
    }

    if(selectSFX != nullptr){
        selectSFX->RequestDelete();
        selectSFX = nullptr;
    }

    delete iconVisited;
    delete selectSFX;
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

                if(selectSFX == nullptr){
                    selectSFX = new GameObject();
                    Sound *selectSFX_sound = new Sound(*selectSFX, SKILL_SELECTION); 
 
                    selectSFX_sound->Play(1);
                }

                if(inputManager.MousePress(LEFT_MOUSE_BUTTON) && canVisited){ 
                    GameObject* selectedSFX = new GameObject();
                    Sound *selectSFX_sound = new Sound(*selectedSFX, SKILL_SELECTION_CONFIRMED); 
                    selectSFX_sound->Play(1);   
                    SetWasVisited(true);

                    //If can be visited go to state of node
                    Map::mapPosition = std::make_pair(floor, column);
                    SetNewStage(type);
                    
                   
                }
            }else{
                nextComponentPtr->SetAlpha(120);
                if(selectSFX != nullptr){
                    selectSFX->RequestDelete();
                    selectSFX = nullptr;
                }
                
                
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

                    nextComponentPtr->SetScale(scaleSprite.x, scaleSprite.x);


                // Postion correction
                associated.box.x = posXenterX - associated.box.w / 2;
                associated.box.y = posXenterY - associated.box.h / 2;
            }
            else{

                // Center position original
                auto posXenterX = associated.box.x + associated.box.w / 2;
                auto posXenterY = associated.box.y + associated.box.h / 2;


                    nextComponentPtr->SetScale(1, 1);


                // Postion correction
                associated.box.x = posXenterX - associated.box.w / 2;
                associated.box.y = posXenterY - associated.box.h / 2;
                
            }
        }    
}


void Node::Render(){
} 


void Node::SetCanVisit(bool canVisited){
    this->canVisited = canVisited;
}

void Node::SetWasVisited(bool wasVisited){
    std::vector<std::pair<int, int>> key{_v1}; // Constructing a vector with _v1 as its only element
    Map::visited_nodes[key] = wasVisited;
}

int Node::GetFloor(){
    return floor;
}

void Node::SetNewStage(NodeType node){



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
            CreateRest();
            break;
        } 
        case NodeType::UNKNOWN: {
            int randomizedEncounter = rand() % 100 + 1;
            //COMBAT
            if(randomizedEncounter < 50){
                CreateCombat(); 
            }
 
            //MURAL
            else if(randomizedEncounter > 80){
                CreateMural();
            }

            //REST
            else if(randomizedEncounter > 100){
                CreateRest();
            }
            else{
                CreateCombat(); 
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
    new Sprite(*iconVisited, MAP_VISITED_SPRITE);


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

void Node::CreateRest(){
    Rest* new_stage = new Rest(); 
    Game::GetInstance().Push(new_stage);  
} 

void Node::CreateCombat(){
    std::vector<Enemies::EnemyId> enemiesArray = GetRandomEncounter(Map::mapPosition.first);                                               

    std::string spriteCombatBackground;

    //encounters from 1-5
    if(Map::mapPosition.first < 6){
        spriteCombatBackground = COMBAT_IMAGE_SEWER;
    }
    //encounters from 6-10
    else if(Map::mapPosition.first < 11){
        spriteCombatBackground = COMBAT_IMAGE_PRISON;
    }
    //encounters from 11-15
    else if(Map::mapPosition.first < 16){
        spriteCombatBackground = COMBAT_IMAGE_TEMPLE;
    }
    //encounter BOSS
    else{
        spriteCombatBackground = COMBAT_IMAGE_TEMPLE;
    }

    CombatState* new_stage = new CombatState(enemiesArray, spriteCombatBackground); 
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
    encounterMap[2] = { Enemies::FrogDad, Enemies::CultistGreen };
    encounterMap[3] = { Enemies::CultistGreen, Enemies::FrogMom };
    encounterMap[4] = { Enemies::FrogDad, Enemies::FrogMom};
    encounterMap[5] = { Enemies::CultistGreen, Enemies::FrogDad, Enemies::FrogMom };

    //encounters from 6-10
    encounterMap[6] = { Enemies::FrogDad, Enemies::CatStone};
    encounterMap[7] = { Enemies::CultistRed, Enemies::CatStone };
    encounterMap[8] = { Enemies::CultistGreen, Enemies::CatGold, Enemies::CatStone };
    encounterMap[9] = { Enemies::CatGold, Enemies::CatStone};
    encounterMap[10] = { Enemies::CultistRed, Enemies::FrogDad, Enemies::CatStone }; 
    
    //encounters from 11-15 
    encounterMap[11] = { Enemies::CultistPurple, Enemies::FrogDad, Enemies::Parakeet};
    encounterMap[12] = { Enemies::CatGold, Enemies::CultistPurple, Enemies::Parakeet };
    encounterMap[13] = { Enemies::CultistPurple, Enemies::CultistRed, Enemies::CultistGreen };
    encounterMap[14] = { Enemies::CultistPurple, Enemies::CultistPurple, Enemies::CatStone};
    encounterMap[15] = { Enemies::CultistPurple, Enemies::CatGold, Enemies::FrogDad };

    encounterMap[16] = { Enemies::Radog, Enemies::CultistRed, Enemies::CultistRed };


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
        return encounterMap[16];
    }

    return encounterMap[1];
}

bool Node::Is(std::string type){
    return (type == "Node");
}

 