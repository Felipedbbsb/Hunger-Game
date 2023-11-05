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

#define DAUGHTER_INDICATOR_SPRITE "assets/img/UI/uiIndicatorPlayer.png"

#define DAUGHTER_INTENTON_SPRITE "assets/img/UI/uiIntentionPlayer.png"

#define DAUGHTER_SPRITE "assets/img/daughter/daughterIdle.png"
#define DAUGHTER_FC 7
#define DAUGHTER_FT 1.0

#define DAUGHTER_HP 27

class Daughter : public Component{

  public:
    Daughter(GameObject &associated);
    ~Daughter();
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
    void ApplySkillToDaughter(int damage, std::vector<Tag::Tags> tags);
    void ApplyTags(std::vector<Tag::Tags> skillTags);
    std::weak_ptr<GameObject>  AddObjTag(Tag::Tags tag);
    bool HasTag(Tag::Tags tagToCheck);
    void ActivateTag(Tag::Tags tag); 

    void RemoveOneTagAll();
    void RecreateTagUI();

    GameObject* indicator; 
    GameObject* intention; 

    static int hp;
    static std::vector<Tag::Tags> tags;

    static bool activateRampage;
    static bool activateWeak;
    static std::weak_ptr<GameObject> daughterInstance;

private:
  LifeBar* lifeBarDaughter;
  Rect daughterHitbox;

  int tagSpaceCount;

  std::vector<std::weak_ptr<GameObject>> daughtertags;//only used for destructor

  std::map<Tag::Tags, int> tagCountMap; // Map to track tag counts, separated from mothertags



};