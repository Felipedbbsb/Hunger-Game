#include "Papiro.h"
#include "Text.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Reader.h"
#include "GameData.h"
#include "CombatState.h"
#include <algorithm> 

Papiro::Papiro(GameObject &associated, std::vector<Enemies::EnemyId> enemiesArrayIS, Skill::AttackType attackType, Skill::TargetType whoAttacks, Skill::TargetType whoReceives)
    : Component(associated),
      papiroVelocity(PAPIRO_VELOCITY),
      enemiesArrayIS(enemiesArrayIS),
      attackType(attackType),
      whoAttacks(whoAttacks),
      whoReceives(whoReceives),
      interactionTime(){
}

void Papiro::Start() {
    // Configure the sprites based on the value of whoAttacks
    std::string spritePath = (whoAttacks == Skill::TargetType::MOTHER || whoAttacks == Skill::TargetType::DAUGHTER) 
        ? PAPIRO_PLAYER_SPRITE
        : PAPIRO_ENEMY_SPRITE;
 
    Sprite *Papiro_spr = new Sprite(associated, spritePath); 
    associated.AddComponent(std::shared_ptr<Sprite>(Papiro_spr));

    if (whoAttacks == Skill::TargetType::MOTHER || whoAttacks == Skill::TargetType::DAUGHTER) {
        // Mother or Daughter is attacking, move Papiro to the right
        associated.box.x -= associated.box.w;
    } else {
        // Other characters are attacking, move Papiro to the left
        associated.box.x += associated.box.w;
    }
}
 
Papiro::~Papiro() { 
}

void Papiro::Update(float dt) {
    // Check if whoAttacks is Mother or Daughter
    if (whoAttacks == Skill::TargetType::MOTHER || whoAttacks == Skill::TargetType::DAUGHTER) {
        // Mother or Daughter is attacking, move Papiro to the right
        associated.box.x += papiroVelocity * dt;
    } else {
        // Other characters are attacking, move Papiro to the left
        associated.box.x -= papiroVelocity * dt;
    }

    interactionTime.Update(dt);
    if (interactionTime.Get() >= INTERACTION_COOLDOWN) {
        interactionTime.Restart();
        associated.RequestDelete();
        CombatState::InteractionSCreenActivate = false;
    }

}

void Papiro::Render() {
}

bool Papiro::Is(std::string type) {
    return (type == "Papiro");
}
