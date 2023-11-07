#include "InteractionObject.h"
#include "Sprite.h"
#include "Component.h"
#include "Enemies.h"
#include "Game.h"
#include "Skill.h"
#include "Enemies.h"
#include "Mother.h"
#include "Daughter.h"


InteractionObject::InteractionObject(GameObject &associated, Skill::AttackType attackType, Skill::TargetType targetType, Enemies::EnemyId enemyId, bool isAttacking)
    : Component::Component(associated), 
    targetType(targetType), 
    enemyId(enemyId),
    attackType(attackType),
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
        if(targetType == Skill::TargetType::MOTHER){
            if(isAttacking){
                if(attackType == Skill::AttackType::BUFF_INDIVIDUAL || attackType == Skill::AttackType::BUFF_ALL){
                    iconPath = MOTHER_SPRITE_BUFF;
                    Sprite* obj_spr = new Sprite(associated, iconPath); 
                    associated.AddComponent(std::shared_ptr<Sprite>(obj_spr));
                }
                else{
                    iconPath = MOTHER_SPRITE_ATK;
                    Sprite* obj_spr = new Sprite(associated, iconPath); 
                    associated.AddComponent(std::shared_ptr<Sprite>(obj_spr));
                }
                
            }
            else{  
                if(attackType == Skill::AttackType::BUFF_INDIVIDUAL || attackType == Skill::AttackType::BUFF_ALL){
                    Sprite *obj_spr = new Sprite(associated, MOTHER_SPRITE, MOTHER_FC, MOTHER_FT/ MOTHER_FC);
                    associated.AddComponent(std::shared_ptr<Sprite>(obj_spr)); 
                    obj_spr->SetScale(1.15, 1.15);
                }
                else{
                    iconPath = MOTHER_SPRITE_DFS;
                    Sprite* obj_spr = new Sprite(associated, iconPath); 
                    associated.AddComponent(std::shared_ptr<Sprite>(obj_spr)); 
                }

            } 
            
        }
        else{
            if(isAttacking){
                if(attackType == Skill::AttackType::BUFF_INDIVIDUAL || attackType == Skill::AttackType::BUFF_ALL){
                    iconPath = DAUGHTER_SPRITE_BUFF;
                    Sprite* obj_spr = new Sprite(associated, iconPath); 
                    associated.AddComponent(std::shared_ptr<Sprite>(obj_spr));
                }
                else{
                    iconPath = DAUGHTER_SPRITE_ATK;
                    Sprite* obj_spr = new Sprite(associated, iconPath); 
                    associated.AddComponent(std::shared_ptr<Sprite>(obj_spr));
                }
                
            } 
            else{
                if(attackType == Skill::AttackType::BUFF_INDIVIDUAL || attackType == Skill::AttackType::BUFF_ALL){
                    Sprite *obj_spr = new Sprite(associated, DAUGHTER_SPRITE, DAUGHTER_FC, DAUGHTER_FT/ DAUGHTER_FC);
                    associated.AddComponent(std::shared_ptr<Sprite>(obj_spr));
                    obj_spr->SetScale(1.15, 1.15);
                }
                else{
                    iconPath = DAUGHTER_SPRITE_DFS;
                    Sprite* obj_spr = new Sprite(associated, iconPath); 
                    associated.AddComponent(std::shared_ptr<Sprite>(obj_spr));
                }
                
            }
        }
        
    }

    
}
 
void InteractionObject::Update(float dt) {

}

void InteractionObject::Render() {

}

bool InteractionObject::Is(std::string type) {
    return (type == "InteractionObject"); 
}