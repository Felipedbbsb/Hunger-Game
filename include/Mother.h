#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"
#include "LifeBar.h"
#include "Skill.h"
#include "Rect.h"

#define MOTHER_INDICATOR_SPRITE "assets/img/UI/uiEnemyIndicator.png"

#define MOTHER_INTENTON_SPRITE "assets/img/UI/uiIntentionPlayer.png"

#define MOTHER_SPRITE "assets/img/mom/momIdle.png"

#define MOTHER_HP 50

class Mother : public Component{

  public:
    Mother(GameObject &associated);
    ~Mother();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();
    bool Is(std::string type);

    void SetupInteractionScreen(Skill::AttackType attackType, Skill::TargetType whoAttacks);

    void CreateIndicator();
    void DeleteIndicator();
    void CreateIntention();
    void DeleteIntention();
    void ApplySkillToMother(int damage, std::vector<Tag::Tags> tags);
    void ApplyTags(std::vector<Tag::Tags> skillTags);
    std::weak_ptr<GameObject>  AddObjTag(Tag::Tags tag);
    bool HasTag(Tag::Tags tagToCheck);
    void ActivateTag(Tag::Tags tag); 

    GameObject* indicator;   
    GameObject* intention; 

    static int hp;
    static std::vector<Tag::Tags> tags;

    static bool activateRampage;
    static bool activateWeak;

private:
    LifeBar* lifeBarMother;
    Rect motherHitbox;

    int tagSpaceCount;

    std::vector<std::weak_ptr<GameObject>> mothertags;//only used for destructor

    std::map<Tag::Tags, int> tagCountMap; // Map to track tag counts, separated from mothertags

    
    

};