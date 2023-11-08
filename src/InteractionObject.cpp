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
    isAttacking(isAttacking),
    effect(nullptr),
    typeEffect("") {
    
}
 
InteractionObject::~InteractionObject() {
}
 
void InteractionObject::Start() { 
    if(targetType == Skill::TargetType::IRR){
        Enemies::EnemyInfo& enemyInfo = Enemies::enemyInfoMap[enemyId]; 
        iconPath = enemyInfo.iconPath;
        Sprite* obj_spr = new Sprite(associated, iconPath); 
        associated.AddComponent(std::shared_ptr<Sprite>(obj_spr));

        if(isAttacking){
            if(attackType == Skill::AttackType::BUFF_INDIVIDUAL || attackType == Skill::AttackType::BUFF_ALL){
                //CREATES BUFF
                CreateEffect("BUFF");
                
            }
            else{
                //CREATES NONE
            }
        }else{
            if(attackType == Skill::AttackType::BUFF_INDIVIDUAL || attackType == Skill::AttackType::BUFF_ALL){
                //CREATES NONE
            }
            else{
                //CREATES DAMAGE OR DEBUFF
                CreateEffect("ATK");
            }
        }
        
        
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
                    obj_spr->SetScale(1.35, 1.35);

                    //CREATES BUFF
                    CreateEffect("BUFF");
                }
                else{
                    iconPath = MOTHER_SPRITE_DFS;
                    Sprite* obj_spr = new Sprite(associated, iconPath); 
                    associated.AddComponent(std::shared_ptr<Sprite>(obj_spr)); 
                    //CREATES DAMAGE OR DEBUFF
                    CreateEffect("ATK");
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
                    obj_spr->SetScale(1.35, 1.35);

                    //CREATES BUFF
                    CreateEffect("BUFF");
                }
                else{
                    iconPath = DAUGHTER_SPRITE_DFS;
                    Sprite* obj_spr = new Sprite(associated, iconPath); 
                    associated.AddComponent(std::shared_ptr<Sprite>(obj_spr));
                    //CREATES DAMAGE OR DEBUFF
                    CreateEffect("ATK");
                }
                
            }
        }
        
    }

    
}

void InteractionObject::CreateEffect(std::string TypeEffect) {
    effect = new GameObject();
    Sprite* effect_spr = nullptr; // Declare a variável aqui

    if (TypeEffect == "BUFF") {
        effect_spr = new Sprite(*effect, BUFF_SPRITE); // Use *effect para obter o objeto por referência
    } else if (TypeEffect == "ATK") {
        effect_spr = new Sprite(*effect, ATK_SPRITE);
    } else if (TypeEffect == "DEBUFF") {
        effect_spr = new Sprite(*effect, DEBUFF_SPRITE);
    }

    // Verifique se effect_spr não é nulo antes de adicioná-lo
    if (effect_spr) { 
        effect->AddComponent(std::shared_ptr<Sprite>(effect_spr));
    }

    effect_spr->SetScale(0.5, 0.5);
    effect->box.x = associated.box.x + associated.box.w/2 - effect->box.w/2;
    effect->box.y = associated.box.y + associated.box.h/2 - effect->box.h/2;

    

    Game::GetInstance().GetCurrentState().AddObject(effect);

    typeEffect = TypeEffect;
}


 
void InteractionObject::Update(float dt) {


}

void InteractionObject::SetPos(int posX, int posY) {
    if(effect != nullptr){
        if(typeEffect == "ATK"){
            effect->box.x = posX + associated.box.w/2 - effect->box.w/2 + 60;
            effect->box.y = posY + associated.box.h/6 - effect->box.h/2;
        } else{
            effect->box.x = posX + associated.box.w/2 - effect->box.w/2;
            effect->box.y = posY + associated.box.h/6 - effect->box.h/2;
        } 
              
    }

}

void InteractionObject::Render() {

}

bool InteractionObject::Is(std::string type) {
    return (type == "InteractionObject"); 
}