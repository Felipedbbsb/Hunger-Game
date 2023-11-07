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
    Sprite *Papiro_spr = new Sprite(*papiro_obj, spritePath); 
    papiro_obj->AddComponent(std::shared_ptr<Sprite>(Papiro_spr));    

    associated.box.w = papiro_obj->box.w;
    associated.box.h = papiro_obj->box.h;

    if (movingRight) {
        // Move Papiro to the left initially
        associated.box.x -= associated.box.w; 
    } else {
        // Move Papiro to the right initially
        associated.box.x += RESOLUTION_WIDTH * Game::resizer;
    }
    papiro_obj->box.x = associated.box.x-Camera::pos.x;
    papiro_obj->box.y = -Camera::pos.y;

    papiroVelocity = 2*(associated.box.w / (INTERACTION_COOLDOWN * 0.25)) ;
    papiroAc =(papiroVelocity) / (INTERACTION_COOLDOWN * 0.25);

    // Posicionar o ponto central 
    //int centerX = 392 + (1132 / 2) + associated.box.x; // Posição X do ponto central
    //int centerY = 221 + (638 / 2); // Posição Y do ponto central
 
 
    background = new GameObject(associated.box.x + 392, 221);
    Sprite *background_spr = new Sprite(*background, spriteBackground);
    background->AddComponent(std::shared_ptr<Sprite>(background_spr));

    background_spr->SetClip(0, 0, PAPIRO_SCREEN.x/BG_SCALE, PAPIRO_SCREEN.y/BG_SCALE); // Define um ponto central de 1x1 pixel
    background_spr->SetScale(BG_SCALE,BG_SCALE);
    //background_spr->SetDesaturation(true); 
    Game::GetInstance().GetCurrentState().AddObject(background);  
 

    //offeset precisely made by sprite reference 
    backgroundOffsetX = 392; 
    if (!movingRight){
        backgroundOffsetX = 393;
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
            CreateEnemyObject();
            CreatePlayerObject(whoAttacks, Skill::TargetType::IRR);
            objectsMovesRight = true;
            centralized = false;
        }
        
    }
    else{
        if(attackType == Skill::BUFF_INDIVIDUAL || attackType == Skill::BUFF_ALL){   
            CreateEnemyObject();
            objectsMovesRight = true;
            centralized = true;
        }
        else{
            CreatePlayerObject(whoReceives, Skill::TargetType::IRR);
            CreateEnemyObject();
            objectsMovesRight = false;
            centralized = false;
        }
    }


    Game::GetInstance().GetCurrentState().AddObject(papiro_obj); //last layer

}   

void Papiro::CreateEnemyObject(){
    if(!enemiesArrayIS.empty()){
        for (const auto &enemyId : enemiesArrayIS) {
            GameObject* interactionObj = new GameObject(background->box.x, background->box.y);
            InteractionObject* interactionObj_behaviour = new InteractionObject(*interactionObj, attackType, Skill::TargetType::IRR, enemyId, !movingRight);
            interactionObj->AddComponent(std::shared_ptr<InteractionObject>(interactionObj_behaviour));
            interactionObj->box.x -= interactionObj->box.w;

            CameraFollower *interactionObj_cmfl = new CameraFollower(*interactionObj);
            interactionObj->AddComponent((std::shared_ptr<CameraFollower>)interactionObj_cmfl);

            std::weak_ptr<GameObject> go_obj = Game::GetInstance().GetCurrentState().AddObject(interactionObj);
            interactionObjects.push_back(go_obj);

        }
    }
}

void Papiro::CreatePlayerObject(Skill::TargetType targetType,  Skill::TargetType targetReceiver){
    GameObject* interactionObjP = new GameObject(background->box.x, background->box.y);
    bool tempRight = movingRight;
    if(targetType == targetReceiver ){
        tempRight = !tempRight;
    }
    InteractionObject* interactionObjP_behaviour = new InteractionObject(*interactionObjP, attackType, targetType, Enemies::EnemyId::INVALID_ENEMY, tempRight);   
    interactionObjP->AddComponent(std::shared_ptr<InteractionObject>(interactionObjP_behaviour));
    interactionObjP->box.x -= interactionObjP->box.w;

    CameraFollower *interactionObjP_cmfl = new CameraFollower(*interactionObjP);
    interactionObjP->AddComponent((std::shared_ptr<CameraFollower>)interactionObjP_cmfl);


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

    background->RequestDelete();
    papiro_obj->RequestDelete();

    

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
         if (interactionTime.Get() < INTERACTION_COOLDOWN * 0.25) {
            associated.box.x += papiroVelocity * dt;
            papiroVelocity -= papiroAc * dt;
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
        if (interactionTime.Get() < INTERACTION_COOLDOWN * 0.25) {
            associated.box.x -= papiroVelocity * dt;
            papiroVelocity -= papiroAc * dt;
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

    background->box.x = associated.box.x + backgroundOffsetX -Camera::pos.x;
    papiro_obj->box.x = associated.box.x -Camera::pos.x;

    if(objectsMovesRight){
        objectsMoves += OBJECT_VELOCITY * dt;
    }
    else{ 
        objectsMoves -= OBJECT_VELOCITY * dt;    
    }  
  
    int spacingEnemies = 0; 
    for (auto& interactionObj : interactionObjects) {
        interactionObj.lock()->box.x = background->box.x + PAPIRO_SCREEN.x/2 - interactionObj.lock()->box.w/2 + objectsMoves + SPACING_ENEMIES * spacingEnemies;

        if(!centralized){
           interactionObj.lock()->box.x += interactionObj.lock()->box.w/2;
        }

        interactionObj.lock()->box.y = background->box.y +  background->box.h - interactionObj.lock()->box.h-Camera::pos.y;
        spacingEnemies++;

    }
 
    int spacingplayers = 0; 
    for (auto& PLayerObject : PLayerObjects) {
        PLayerObject.lock()->box.x = background->box.x + PAPIRO_SCREEN.x/2 - PLayerObject.lock()->box.w/2 + objectsMoves + SPACING_PLAYERS * spacingplayers;
        
        if(!centralized){
           PLayerObject.lock()->box.x += PAPIRO_SCREEN.x/4 - PAPIRO_SCREEN.x/2 ;
        }

        PLayerObject.lock()->box.y = background->box.y +  background->box.h - PLayerObject.lock()->box.h-Camera::pos.y;
        spacingplayers++;

    }

    interactionTime.Update(dt); 
}

void Papiro::Render() {
}
 
bool Papiro::Is(std::string type) { 
    return (type == "Papiro");
}
