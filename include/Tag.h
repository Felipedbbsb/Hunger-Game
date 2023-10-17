#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"

#include "Skill.h"

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
        Tag(GameObject &associated, 
                Skill::SkillsTags tag, 
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

        Skill::SkillsTags GetTag();

        void ShowReader();
        void HideReader();
        std::string GetTagMessage();
        std::string GetTagName();

    private:
        std::weak_ptr<GameObject> enemyRef; 
        int quantity;
        Skill::SkillsTags tag;
        GameObject* textSkillObj;
        std::string textSkill;
        GameObject* readerTag;
};