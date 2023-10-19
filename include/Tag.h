#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"


#define TAG_RESILIENCE_SPRITE "assets/img/UI/uiIconResilience.png"
#define TAG_DODGE_SPRITE "assets/img/UI/uiIconDodge.png"
#define TAG_PROVOKE_SPRITE "assets/img/UI/uiIconProvoke.png"
#define TAG_VULNERABLE_SPRITE "assets/img/UI/uiIconVulnerable.png"
#define TAG_WEAK_SPRITE "assets/img/UI/uiIconWeak.png"
#define TAG_RAMPAGE_SPRITE "assets/img/UI/uiIconRampage.png"
#define TAG_PROTECTED_SPRITE "assets/img/UI/uiIconProtected.png"

#define TAGS_SPACING_X 20.0f
#define TAGS_SPACING_Y 35.0f


class Tag : public Component{

    public:

 
        enum Tags {
            RESILIENCE, //Reduz o dano recebido em 50% (DONE)
            WEAK,//Reduz o seu dano em 50%
            RAMPAGE,//Aumenta seu dano em 50% 
            VULNERABLE,//Aumenta o dano recebido em 50% (DONE)
            PROVOKE,//Força os inimigos a atacarem este alvo (DONE)
            DODGE, //50% de chance de evitar todo o dano do próximo golpe
            PROTECTED//Não pode ser alvejado
        };

        Tag(GameObject &associated, 
                Tags tag, 
                std::weak_ptr<GameObject> enemyRef,
                int quantity);
        
        


        ~Tag();
        void Update(float dt);
        void UpdateQuantity(int newQuantity);
        void Render(); 
        void Start();
        void Pause();
        void Resume();
        bool Is(std::string type);

        Tag::Tags GetTag();

        void AcivateTag(std::string sprite);
        void ShowReader();
        void HideReader();
        std::string GetTagMessage();
        std::string GetTagName();
        std::string GetTagSprite(Tag::Tags tag);
         
    private:
        std::weak_ptr<GameObject> enemyRef; 
        int quantity;
        Tags tag;
        GameObject* textSkillObj;
        std::string textSkill;
        GameObject* readerTag;
        std::shared_ptr<Sprite> bigTag;
        
};