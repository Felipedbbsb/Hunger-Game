#include "InteractionObject.h"
#include "Sprite.h"
#include "Component.h"
#include "Enemies.h"
#include "Game.h"
#include "Skill.h"
#include "Enemies.h"


InteractionObject::InteractionObject(GameObject &associated, Skill::TargetType targetType, Enemies::EnemyId enemyId, bool isAttacking)
    : Component::Component(associated), 
    targetType(targetType), 
    enemyId(enemyId),
    isAttacking(isAttacking) {
    
}
 
InteractionObject::~InteractionObject() {
}
 
void InteractionObject::Start() { 
    if(targetType == Skill::TargetType::IRR){
        Enemies::EnemyInfo& enemyInfo = Enemies::enemyInfoMap[enemyId]; 
        iconPath = enemyInfo.iconPath;
         Sprite* obj_spr = new Sprite(associated, iconPath); 
        associated.AddComponent(std::shared_ptr<Sprite>(obj_spr));
        
    }
    else{
        std::cout << "aaaaa" << std::endl;
        if(targetType == Skill::TargetType::MOTHER){
            if(isAttacking){
                iconPath = MOTHER_SPRITE_ATK;
            }
            else{ 
                iconPath = MOTHER_SPRITE_DFS;
            } 
            
        }
        else{
            if(isAttacking){
                iconPath = DAUGHTER_SPRITE_ATK;
            }
            else{
                iconPath = DAUGHTER_SPRITE_DFS;
            }
        }
        Sprite* obj_spr = new Sprite(associated, iconPath); 
        associated.AddComponent(std::shared_ptr<Sprite>(obj_spr));
    }

    
}
 
void InteractionObject::Update(float dt) {

}

void InteractionObject::Render() {

}

bool InteractionObject::Is(std::string type) {
    return (type == "InteractionObject"); 
}