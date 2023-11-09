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
    if(effect != nullptr){
        effect->RequestDelete();
        effect = nullptr;
    }
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
                CreateEffect("BUFF", false);
                
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
                if(attackType == Skill::AttackType::DEBUFF_INDIVIDUAL || attackType == Skill::AttackType::DEBUFF_ALL){
                    CreateEffect("DEBUFF", false); 
                }else{
                    CreateEffect("ATK", false);
                }    
                
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
                    CreateEffect("BUFF", true);
                }
                else{
                    iconPath = MOTHER_SPRITE_DFS;
                    Sprite* obj_spr = new Sprite(associated, iconPath); 
                    associated.AddComponent(std::shared_ptr<Sprite>(obj_spr)); 
                    //CREATES DAMAGE OR DEBUFF
                    if(attackType == Skill::AttackType::DEBUFF_INDIVIDUAL || attackType == Skill::AttackType::DEBUFF_ALL){
                    CreateEffect("DEBUFF", true); 
                    }else{
                        CreateEffect("ATK", true);
                    };
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
                    CreateEffect("BUFF", true);
                }
                else{
                    iconPath = DAUGHTER_SPRITE_DFS;
                    Sprite* obj_spr = new Sprite(associated, iconPath); 
                    associated.AddComponent(std::shared_ptr<Sprite>(obj_spr));
                    //CREATES DAMAGE OR DEBUFF
                    if(attackType == Skill::AttackType::DEBUFF_INDIVIDUAL || attackType == Skill::AttackType::DEBUFF_ALL){
                    CreateEffect("DEBUFF", true);
                    }else{
                        CreateEffect("ATK", true);
                    }
                }
                
            }
        }
        
    }

    
}

void InteractionObject::CreateEffect(std::string TypeEffect, bool isPlayer) {
    effect = new GameObject();
    Sprite* effect_spr = nullptr; // Declare a variável aqui

    if(isPlayer){
        if (TypeEffect == "BUFF") {
            effect_spr = new Sprite(*effect, BUFF_SPRITE_R); 
        } else if (TypeEffect == "ATK") {
            effect_spr = new Sprite(*effect, ATK_SPRITE_R);
        } else if (TypeEffect == "DEBUFF") {
            effect_spr = new Sprite(*effect, DEBUFF_SPRITE_R);
        }
    }
    else{
        if (TypeEffect == "BUFF") {
            effect_spr = new Sprite(*effect, BUFF_SPRITE); 
        } else if (TypeEffect == "ATK") {
            effect_spr = new Sprite(*effect, ATK_SPRITE);
        } else if (TypeEffect == "DEBUFF") {
            effect_spr = new Sprite(*effect, DEBUFF_SPRITE);
        }
    }
    

    // Verifique se effect_spr não é nulo antes de adicioná-lo 
    if (effect_spr) { 
        effect->AddComponent(std::shared_ptr<Sprite>(effect_spr)); 
        effect_spr->SetAlpha(1);
    }

    effect->box.x = associated.box.x + associated.box.w/2 - effect->box.w/2;
    effect->box.y = associated.box.y + associated.box.h/2 - effect->box.h/2;
    

    Game::GetInstance().GetCurrentState().AddObject(effect);

    typeEffect = TypeEffect;
}


 
void InteractionObject::Update(float dt) {
    if(effect != nullptr){
        effectDuration.Update(dt);

        auto velocityAplha = 255 / (INTERACTION_COOLDOWN * 0.75);

        auto objComponent = effect->GetComponent("Sprite");
        auto objComponentPtr = std::dynamic_pointer_cast<Sprite>(objComponent);
        if(objComponentPtr){
            auto aplhaSprite = objComponentPtr->GetAlpha();
            std::cout << aplhaSprite << std::endl; 
            if( effectDuration.Get() < INTERACTION_COOLDOWN * 0.8 ){
                if(aplhaSprite <= 255){
                    objComponentPtr->SetAlpha(aplhaSprite +  velocityAplha * dt);
                    if(objComponentPtr->GetAlpha() > 255){
                        objComponentPtr->SetAlpha(255);
                    }  
                }
                
            } 
            else if(effectDuration.Get() > INTERACTION_COOLDOWN * 1.0){
                if(aplhaSprite > 1){
                    objComponentPtr->SetAlpha(aplhaSprite -  velocityAplha * dt);
                    if(objComponentPtr->GetAlpha() < 0){
                        objComponentPtr->SetAlpha(1);
                    }
                }
            }
        }
        
    }
}

void InteractionObject::SetPos(int posX, int posY) {
    if(effect != nullptr){
        if(typeEffect == "ATK"){
            effect->box.x = posX + associated.box.w/2 - effect->box.w/2 - 10;
            effect->box.y = posY + associated.box.h/3 - effect->box.h/2;
        }
        else if(typeEffect == "BUFF") {
            effect->box.x = posX + associated.box.w/2 - effect->box.w/2;
            effect->box.y = posY + associated.box.h - effect->box.h - 10;
        } 
        else if(typeEffect == "DEBUFF") {
            effect->box.x = posX + associated.box.w/2 - effect->box.w/2;
            effect->box.y = posY + associated.box.h/2 - effect->box.h/2;
        } 
              
    }

    


}
 
void InteractionObject::Render() {

}

bool InteractionObject::Is(std::string type) {
    return (type == "InteractionObject"); 
}