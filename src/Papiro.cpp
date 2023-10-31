#include "Papiro.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Reader.h"
#include "GameData.h"
#include "CombatState.h"
#include "InteractionObject.h"
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
      spriteBackground(spriteBackground){
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
        associated.box.x += RESOLUTION_WIDTH;
    }
    papiro_obj->box.x = associated.box.x;

    papiroVelocity = 2*(associated.box.w / (INTERACTION_COOLDOWN * 0.25)) ;
    papiroAc =(papiroVelocity) / (INTERACTION_COOLDOWN * 0.25);

    // Posicionar o ponto central 
    //int centerX = 392 + (1132 / 2) + associated.box.x; // Posição X do ponto central
    //int centerY = 221 + (638 / 2); // Posição Y do ponto central

 
    background = new GameObject(associated.box.x + 392, 221);
    Sprite *background_spr = new Sprite(*background, spriteBackground);
    background->AddComponent(std::shared_ptr<Sprite>(background_spr));
    
    background_spr->SetClip(associated.box.w/4, associated.box.h/4, 1132/BG_SCALE, 638/BG_SCALE); // Define um ponto central de 1x1 pixel
    background_spr->SetScale(BG_SCALE,BG_SCALE);
    //background_spr->SetDesaturation(true);
    Game::GetInstance().GetCurrentState().AddObject(background);

    Game::GetInstance().GetCurrentState().AddObject(papiro_obj);

    //offeset precisely made by sprite reference 
    backgroundOffsetX = 392; 
    if (!movingRight){
        backgroundOffsetX = 393;
    }  




    //Combat interection objects
    for (const auto &enemyId : enemiesArrayIS) {
        GameObject* interactionObj = new GameObject();
        InteractionObject* interactionObj_behaviour = new InteractionObject(*interactionObj, whoAttacks, enemyId);
        interactionObj->AddComponent(std::shared_ptr<InteractionObject>(interactionObj_behaviour));

        std::weak_ptr<GameObject> go_obj = Game::GetInstance().GetCurrentState().AddObject(interactionObj);
        interactionObjects.push_back(go_obj);

    }





}   

Papiro::~Papiro() {
    for (int i = interactionObjects.size() - 1; i >= 0; i--) { //remove enemies tags
            interactionObjects.erase(interactionObjects.begin() + i);
    }

    background->RequestDelete();
    papiro_obj->RequestDelete();
}
 
void Papiro::Update(float dt) {
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
                papiroVelocity += papiroAc * dt;
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
                papiroVelocity += papiroAc * dt;
                if(interactionTime.Get() >= INTERACTION_COOLDOWN){
                    associated.RequestDelete(); // Remove when it disappears
                    CombatState::InteractionSCreenActivate = false; 
                }
            }
        } 
   
    }

    background->box.x = associated.box.x + backgroundOffsetX;
    papiro_obj->box.x = associated.box.x;


    for (auto& interactionObj : interactionObjects) {
        interactionObj.lock()->box.x = background->box.x;
    }

    interactionTime.Update(dt); 
}

void Papiro::Render() {
}

bool Papiro::Is(std::string type) {
    return (type == "Papiro");
}
