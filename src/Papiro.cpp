#include "Papiro.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Reader.h"
#include "GameData.h"
#include "CombatState.h"
#include "InteractionObject.h"
#include "CameraFollower.h"

#include <algorithm>


Papiro::Papiro(GameObject &associated, std::string spriteBackground, std::vector<Enemies::EnemyId> enemiesArrayIS, Skill::AttackType attackType, Skill::TargetType whoAttacks, Skill::TargetType whoReceives)
    : Component(associated),
      background(nullptr),
      papiro_obj(nullptr),
      enemiesArrayIS(enemiesArrayIS),
      attackType(attackType),
      whoAttacks(whoAttacks),
      whoReceives(whoReceives),
      interactionTime(),
      movingRight(whoAttacks == Skill::TargetType::MOTHER || whoAttacks == Skill::TargetType::DAUGHTER),
      spriteBackground(spriteBackground),
      objectsMoves(0),
      centralized(false){
}
  
void Papiro::Start() {
    // Configure the sprites based on the value of whoAttacks
    std::string spritePath = (movingRight) 
        ? PAPIRO_PLAYER_SPRITE
        : PAPIRO_ENEMY_SPRITE;

    papiro_obj = new GameObject();
    new Sprite(*papiro_obj, spritePath); 

    associated.box.w = papiro_obj->box.w;
    associated.box.h = papiro_obj->box.h;

    if (movingRight) {
        // Move Papiro to the left initially
        associated.box.x -= RESOLUTION_WIDTH;
        papiroVelocity = (2*(RESOLUTION_WIDTH + PAPIRO_OFFSET) / (INTERACTION_COOLDOWN * 0.25));
    } else {
        // Move Papiro to the right initially
        associated.box.x += RESOLUTION_WIDTH ;
        papiroVelocity = (2*(RESOLUTION_WIDTH + PAPIRO_OFFSET) / (INTERACTION_COOLDOWN * 0.25)) ;
    }
    papiro_obj->box.x = associated.box.x-Camera::pos.x;
    papiro_obj->box.y = -Camera::pos.y;
    
    
    papiroAc =(papiroVelocity) / (INTERACTION_COOLDOWN * 0.25);

    // Posicionar o ponto central 
    //int centerX = 392 + (1132 / 2) + associated.box.x; // Posição X do ponto central
    //int centerY = 221 + (638 / 2); // Posição Y do ponto central
 
 
    background = new GameObject(associated.box.x , 221);
    Sprite *background_spr = new Sprite(*background, spriteBackground);

    background_spr->SetClip(0, 0, PAPIRO_SCREEN.x/BG_SCALE, PAPIRO_SCREEN.y/BG_SCALE); // Define um ponto central de 1x1 pixel
    background_spr->SetScale(BG_SCALE,BG_SCALE);
    //background_spr->SetDesaturation(true); 
    Game::GetInstance().GetCurrentState().AddObject(background);  
 

    //offeset precisely made by sprite reference    
    backgroundOffsetX = -840;  
    if (!movingRight){ 
        backgroundOffsetX = 1025;
    }  
 

    //Processing relation between objects
    if(movingRight){
        if(attackType == Skill::BUFF_INDIVIDUAL || attackType == Skill::BUFF_ALL){
            //daughter always in front
            if(whoAttacks == Skill::TargetType::MOTHER){
                CreatePlayerObject(whoAttacks, whoReceives);
                CreatePlayerObject(whoReceives, whoReceives);
            }
            else{
                CreatePlayerObject(whoReceives, whoReceives);
                CreatePlayerObject(whoAttacks, whoReceives);
            }

            objectsMovesRight = false;
            centralized = true;
        }
        else{
            CreateEnemyObject(false);
            CreatePlayerObject(whoAttacks, Skill::TargetType::IRR);
            objectsMovesRight = true;
            centralized = false;
        }
        
    }
    else{
        if(attackType == Skill::BUFF_INDIVIDUAL || attackType == Skill::BUFF_ALL){   
            CreateEnemyObject(true);
            objectsMovesRight = true;
            centralized = true;
        }
        else{
            CreatePlayerObject(whoReceives, Skill::TargetType::IRR);
            CreateEnemyObject(true);
            objectsMovesRight = false;
            centralized = false;
        }
    }

 
    Game::GetInstance().GetCurrentState().AddObject(papiro_obj); //last layer

}   

void Papiro::CreateEnemyObject(bool acting){
    if(!enemiesArrayIS.empty()){
        for (const auto &enemyId : enemiesArrayIS) {
            GameObject* interactionObj = new GameObject(-10000,-10000);
            new InteractionObject(*interactionObj, attackType, Skill::TargetType::IRR, enemyId, acting);
            
            interactionObj->box.x -= interactionObj->box.w;

            std::weak_ptr<GameObject> go_obj = Game::GetInstance().GetCurrentState().AddObject(interactionObj);
            interactionObjects.push_back(go_obj);

        }
    }
}

void Papiro::CreatePlayerObject(Skill::TargetType targetType,  Skill::TargetType targetReceiver){
    GameObject* interactionObjP = new GameObject(-10000,-10000);
    bool tempRight = movingRight;
    if(targetType == targetReceiver ){
        tempRight = !tempRight;
    }
    new InteractionObject(*interactionObjP, attackType, targetType, Enemies::EnemyId::INVALID_ENEMY, tempRight);   
    interactionObjP->box.x -= interactionObjP->box.w; 


    std::weak_ptr<GameObject> go_objP = Game::GetInstance().GetCurrentState().AddObject(interactionObjP);
    PLayerObjects.push_back(go_objP);

}

Papiro::~Papiro() {
    for (int i = PLayerObjects.size() - 1; i >= 0; i--) {
        PLayerObjects[i].lock()->RequestDelete();
    }

    for (int i = interactionObjects.size() - 1; i >= 0; i--) {
        interactionObjects[i].lock()->RequestDelete();
    }
    

    if(background != nullptr){
        background->RequestDelete();
        background = nullptr;
    }

    if(papiro_obj != nullptr){
        papiro_obj->RequestDelete();
        papiro_obj = nullptr;
    }

    delete background;
    delete papiro_obj;

    CombatState::enemiesArrayIS = {};
    
    CombatState::attackType = Skill::AttackType::NONE;

    CombatState::whoAttacks = Skill::TargetType::IRR;

    CombatState::whoReceives = Skill::TargetType::IRR;
}
 
void Papiro::Update(float dt) {


    for (int i = PLayerObjects.size() - 1; i >= 0; i--) {
        PLayerObjects[i].lock()->Update(dt);
    }

    for (int i = interactionObjects.size() - 1; i >= 0; i--) {
        interactionObjects[i].lock()->Update(dt);
    }

    papiro_obj->Update(dt);
 
    if (movingRight) {
        // Mother or Daughter is attacking, move Papiro to the right 
         if (interactionTime.Get() < INTERACTION_COOLDOWN * 0.25 && associated.box.x != RESOLUTION_WIDTH  - papiro_obj->box.w + PAPIRO_OFFSET -Camera::pos.x) {
            associated.box.x += papiroVelocity * dt;
            papiroVelocity -= papiroAc * dt;
            if(associated.box.x >= RESOLUTION_WIDTH  - papiro_obj->box.w + PAPIRO_OFFSET -Camera::pos.x){
                associated.box.x = RESOLUTION_WIDTH  - papiro_obj->box.w + PAPIRO_OFFSET -Camera::pos.x;
            }
        }
        
        else{
            if(interactionTime.Get() >= INTERACTION_COOLDOWN * 0.75){
                associated.box.x += papiroVelocity * dt;
                papiroVelocity -= papiroAc * dt;
                if(interactionTime.Get() >= INTERACTION_COOLDOWN){
                    associated.RequestDelete(); // Remove when it disappears
                    CombatState::InteractionSCreenActivate = false; 
                }
            } 
        } 

       
    } else { 
        // Other characters are attacking, move Papiro to the left
        if (interactionTime.Get() < INTERACTION_COOLDOWN * 0.25  && associated.box.x != -PAPIRO_OFFSET -Camera::pos.x) {
            associated.box.x -= papiroVelocity * dt;
            papiroVelocity -= papiroAc * dt; 
            if(associated.box.x <= -PAPIRO_OFFSET -Camera::pos.x){ 
                associated.box.x = -PAPIRO_OFFSET -Camera::pos.x;
            }
        } 
        else{
            if(interactionTime.Get() >= INTERACTION_COOLDOWN * 0.75){
                associated.box.x -= papiroVelocity * dt;
                papiroVelocity -= papiroAc * dt;
                if(interactionTime.Get() >= INTERACTION_COOLDOWN){
                    associated.RequestDelete(); // Remove when it disappears
                    CombatState::InteractionSCreenActivate = false; 
                }
            }
        } 
   
    } 

    
    background->box.x = associated.box.x + backgroundOffsetX;
    papiro_obj->box.x = associated.box.x;

    if(objectsMovesRight){
        objectsMoves += OBJECT_VELOCITY * dt;
    }
    else{ 
        objectsMoves -= OBJECT_VELOCITY * dt;    
    }  
  
  

    //=================================Enemies update position=============================
    int spacingEnemies = 0; 
    for (int i = interactionObjects.size() - 1; i >= 0; i--) {
        
        interactionObjects[i].lock()->box.x = background->box.x + PAPIRO_SCREEN.x/2 - interactionObjects[i].lock()->box.w/2 + objectsMoves + SPACING_ENEMIES * spacingEnemies + 50;

        if(!centralized){
           interactionObjects[i].lock()->box.x += PAPIRO_SCREEN.x/6 ;
        }

        interactionObjects[i].lock()->box.y = background->box.y +  background->box.h - interactionObjects[i].lock()->box.h-Camera::pos.y;
        spacingEnemies++; 
 

        auto objComponent = interactionObjects[i].lock()->GetComponent("InteractionObject");
        auto objComponentPtr = std::dynamic_pointer_cast<InteractionObject>(objComponent);
        if(objComponentPtr){
            objComponentPtr->SetPos(interactionObjects[i].lock()->box.x, interactionObjects[i].lock()->box.y);
            objComponentPtr->Update(dt);
        } 
        
    }  

    
    //=================================Players update position=============================
    int spacingplayers = 0; 
    for (int i = PLayerObjects.size() - 1; i >= 0; i--) {
        
        PLayerObjects[i].lock()->box.x = background->box.x + PAPIRO_SCREEN.x/2 - PLayerObjects[i].lock()->box.w/2 + objectsMoves + SPACING_PLAYERS * spacingplayers;
        
        if(!centralized){
           PLayerObjects[i].lock()->box.x -= PAPIRO_SCREEN.x/6 ;
        }

        PLayerObjects[i].lock()->box.y = background->box.y +  background->box.h - PLayerObjects[i].lock()->box.h-Camera::pos.y;
        spacingplayers++;
 

        auto objComponent2 = PLayerObjects[i].lock()->GetComponent("InteractionObject");
        auto objComponentPtr2 = std::dynamic_pointer_cast<InteractionObject>(objComponent2);
        if(objComponentPtr2){
            objComponentPtr2->SetPos(PLayerObjects[i].lock()->box.x, PLayerObjects[i].lock()->box.y);
            objComponentPtr2->Update(dt);
        }
    }

    interactionTime.Update(dt); 
}

void Papiro::Render() {
}
 
bool Papiro::Is(std::string type) { 
    return (type == "Papiro");
}
