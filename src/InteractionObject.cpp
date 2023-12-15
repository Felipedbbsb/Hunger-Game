#include "InteractionObject.h"
#include "Sprite.h"
#include "Component.h"
#include "Enemies.h"
#include "Game.h"
#include "Skill.h"
#include "Enemies.h"
#include "Mother.h"
#include "Daughter.h"
#include "GameData.h"

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
    delete effect;
}
 
void InteractionObject::Start() { 
    if(targetType == Skill::TargetType::IRR){
        Enemies::EnemyInfo& enemyInfo = Enemies::enemyInfoMap[enemyId]; 
        

        if(isAttacking){
            if(attackType == Skill::AttackType::BUFF_INDIVIDUAL || attackType == Skill::AttackType::BUFF_ALL){
                iconPath = enemyInfo.iconPathDFS;
                new Sprite(associated, iconPath); 
                
                //CREATES BUFF
                CreateEffect("BUFF", false);
                
                
            }
            else{
                iconPath = enemyInfo.iconPathATK;
                new Sprite(associated, iconPath); 

                //CREATES NONE
            }
        }else{
            if(attackType == Skill::AttackType::BUFF_INDIVIDUAL || attackType == Skill::AttackType::BUFF_ALL){
                iconPath = enemyInfo.iconPathDFS;
                new Sprite(associated, iconPath); 
                
                //CREATES BUFF
                CreateEffect("BUFF", false);
            }
            else{
                iconPath = enemyInfo.iconPathDFS;
                new Sprite(associated, iconPath); 
                

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
        //Mother is attacking
        if(targetType == Skill::TargetType::MOTHER){
            if(isAttacking){
                if(attackType == Skill::AttackType::BUFF_INDIVIDUAL || attackType == Skill::AttackType::BUFF_ALL){
                    
                    if(GameData::npLevel == 0){
                        iconPath =  MOTHER_SPRITE_BUFF;
                    }
                    else if(GameData::npLevel == 1){
                        iconPath =  MOTHER_SPRITE_BUFF_NP1;
                    } 
                    else if(GameData::npLevel == 2){ 
                        iconPath =  MOTHER_SPRITE_BUFF_NP2;
                    }
                    else{
                        iconPath =  MOTHER_SPRITE_BUFF_NP2;
                    }

                    new Sprite(associated, iconPath); 
                }
                else{

                    if(GameData::npLevel == 0){
                        iconPath =  MOTHER_SPRITE_ATK;
                    }
                    else if(GameData::npLevel == 1){
                        iconPath =  MOTHER_SPRITE_ATK_NP1;
                    } 
                    else if(GameData::npLevel == 2){  
                        iconPath =  MOTHER_SPRITE_ATK_NP2;
                    }
                    else{
                        iconPath =  MOTHER_SPRITE_ATK_NP2;
                    }

                    new Sprite(associated, iconPath); 

                }
                 
            }
            else{  
                if(attackType == Skill::AttackType::BUFF_INDIVIDUAL || attackType == Skill::AttackType::BUFF_ALL){

                    if(GameData::npLevel == 0){
                        iconPath =  MOTHER_SPRITE_BUFF;
                    }
                    else if(GameData::npLevel == 1){
                        iconPath =  MOTHER_SPRITE_BUFF_NP1;
                    } 
                    else if(GameData::npLevel == 2){ 
                        iconPath =  MOTHER_SPRITE_BUFF_NP2;
                    }
                    else{
                        iconPath =  MOTHER_SPRITE_BUFF_NP2;
                    }

                    new Sprite(associated, iconPath); 

                    //CREATES BUFF
                    CreateEffect("BUFF", true);
                }
                else{
                    if(GameData::npLevel == 0){
                        iconPath =  MOTHER_SPRITE_DFS;
                    }
                    else if(GameData::npLevel == 1){
                        iconPath =  MOTHER_SPRITE_DFS_NP1;
                    } 
                    else if(GameData::npLevel == 2){ 
                        iconPath =  MOTHER_SPRITE_DFS_NP2;
                    }
                    else{ 
                        iconPath =  MOTHER_SPRITE_DFS_NP2;
                    }

                    new Sprite(associated, iconPath); 

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
                    new Sprite(associated, iconPath); 

                }
                else{
                    iconPath = DAUGHTER_SPRITE_ATK;
                    new Sprite(associated, iconPath); 

                }
                
            } 
            else{
                if(attackType == Skill::AttackType::BUFF_INDIVIDUAL || attackType == Skill::AttackType::BUFF_ALL){

                    iconPath = DAUGHTER_SPRITE_BUFF;
                    new Sprite(associated, iconPath); 

                    //CREATES BUFF
                    CreateEffect("BUFF", true);
                }
                else{
                    iconPath = DAUGHTER_SPRITE_DFS;
                    new Sprite(associated, iconPath); 

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
    Sprite* effect_spr = nullptr; 
    Sound *effect_sound = nullptr;
    if(isPlayer){
        if (TypeEffect == "BUFF") {
            effect_spr = new Sprite(*effect, BUFF_SPRITE_R); 
            effect_sound = new Sound(*effect, BUFF_SOUND);
    
        } else if (TypeEffect == "ATK") {
            effect_spr = new Sprite(*effect, ATK_SPRITE_R);
            effect_sound = new Sound(*effect, ATK_SOUND);
        } else if (TypeEffect == "DEBUFF") {
            effect_spr = new Sprite(*effect, DEBUFF_SPRITE_R);
            effect_sound = new Sound(*effect, DEBUFF_SOUND);
        }
    }
    else{
        if (TypeEffect == "BUFF") {
            effect_spr = new Sprite(*effect, BUFF_SPRITE); 
            effect_sound = new Sound(*effect, BUFF_SOUND);
        } else if (TypeEffect == "ATK") {
            effect_spr = new Sprite(*effect, ATK_SPRITE);
            effect_sound = new Sound(*effect, ATK_SOUND);
        } else if (TypeEffect == "DEBUFF") {
            effect_spr = new Sprite(*effect, DEBUFF_SPRITE);
            effect_sound = new Sound(*effect, DEBUFF_SOUND); 
        }
    }
    

    // Verifique se effect_spr não é nulo antes de adicioná-lo 
    if (effect_spr) { 
        effect_spr->SetAlpha(1);
    }



    effect->box.x = associated.box.x + associated.box.w/2 - effect->box.w/2;
    effect->box.y = associated.box.y + associated.box.h/2 - effect->box.h/2;
    

    Game::GetInstance().GetCurrentState().AddObject(effect);
    effect_sound->Play();
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
            if(targetType == Skill::TargetType::MOTHER){
                effect->box.x = posX + associated.box.w/2 + 70;
                effect->box.y = posY + associated.box.h/2 - effect->box.h/2;
            }
        }
        else if(typeEffect == "BUFF") {
            effect->box.x = posX + associated.box.w/2 - effect->box.w/2;
            effect->box.y = posY + associated.box.h - effect->box.h - 10;

        } 
        else if(typeEffect == "DEBUFF") {
            effect->box.x = posX + associated.box.w/2 - effect->box.w/2;
            effect->box.y = posY + associated.box.h/2 - effect->box.h/2;
            if(targetType == Skill::TargetType::MOTHER){
                effect->box.x = posX + associated.box.w/2 + 70;
                effect->box.y = posY + associated.box.h/2 - effect->box.h/2;
            }
        } 

              
    }

    


}
 
void InteractionObject::Render() {

}

bool InteractionObject::Is(std::string type) {
    return (type == "InteractionObject"); 
}