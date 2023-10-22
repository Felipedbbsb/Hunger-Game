#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "GameObject.h"
#include "Component.h"
#include "Sprite.h"
#include "LifeBar.h"
#include "Skill.h"
#include "Rect.h"



class CombatObject : public Component {
    public:
        CombatObject(GameObject& associated);
 
        ~CombatObject();
        void Update(float dt);
        void Render();
        void Start();
        bool Is(std::string type); 


        void ApplySkill( Skill::SkillInfo& skillInfo);
        void ApplyTags(std::vector<Tag::Tags> skillTags);
        std::weak_ptr<GameObject>  AddObjTag(Tag::Tags tag);
        bool HasTag(Tag::Tags tagToCheck);
        void ActivateTag(Tag::Tags tag); 

        Rect enemyHitbox;
        
        //Enemie stats
        int hp;
        std::vector<Tag::Tags> tags;
    
    private:
        LifeBar* lifeBar;
        

        int tagSpaceCount;

        


        std::vector<std::weak_ptr<GameObject>> combatObjTags;//only used for destructor
        
        std::map<Tag::Tags, int> tagCountMap; // Map to track tag counts, separated from combatObjTags


};
