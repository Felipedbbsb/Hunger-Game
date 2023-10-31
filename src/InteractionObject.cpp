#include "InteractionObject.h"
#include "Sprite.h"
#include "Component.h"
#include "Enemies.h"
#include "Game.h"
#include "Skill.h"
#include "Enemies.h"


InteractionObject::InteractionObject(GameObject &associated, Skill::TargetType targetType, Enemies::EnemyId enemyId)
    : Component::Component(associated), 
    targetType(targetType), 
    enemyId(enemyId) {

    Enemies::EnemyInfo& enemyInfo = Enemies::enemyInfoMap[enemyId]; 
    iconPath = enemyInfo.iconPath;
}
 
InteractionObject::~InteractionObject() {
}
 
void InteractionObject::Start() {
    Sprite* enemies_spr = new Sprite(associated, iconPath); 
    associated.AddComponent(std::shared_ptr<Sprite>(enemies_spr)); 
}
 
void InteractionObject::Update(float dt) {

}

void InteractionObject::Render() {

}

bool InteractionObject::Is(std::string type) {
    return (type == "InteractionObject"); 
}