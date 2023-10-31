#include "Papiro.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Reader.h"
#include "GameData.h"
#include "CombatState.h"
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

 
    background = new GameObject(associated.box.w * 3/4, associated.box.h/4 - 80);
    Sprite *background_spr = new Sprite(*background, spriteBackground);
    background->AddComponent(std::shared_ptr<Sprite>(background_spr));
    
    background_spr->SetClip(associated.box.w/4, associated.box.h/4, background_spr->GetWidth() * 0.75, background_spr->GetHeight() * 0.75); // Define um ponto central de 1x1 pixel
    background_spr->SetScale(BG_SCALE,BG_SCALE);
    //background_spr->SetDesaturation(true);
    Game::GetInstance().GetCurrentState().AddObject(background);

    Game::GetInstance().GetCurrentState().AddObject(papiro_obj);

    backgroundOffsetX = 0;
    if (!movingRight){
        background->box.x += 500;
    }

}  

Papiro::~Papiro() {
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
    interactionTime.Update(dt); 
}

void Papiro::Render() {
}

bool Papiro::Is(std::string type) {
    return (type == "Papiro");
}
