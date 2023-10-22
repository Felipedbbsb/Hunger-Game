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

#define DAUGHTER_INDICATOR_SPRITE "assets/img/UI/uiEnemyIndicator.png"

#define DAUGHTER_INTENTON_SPRITE "assets/img/UI/uiIntentionPlayer.png"

#define DAUGHTER_SPRITE "assets/img/daughter/daughterIdle.png"

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
    
    void CreateIndicator();
    void DeleteIndicator();
    void ApplySkillToDaughter(int damage, std::vector<Tag::Tags> tags);
    void ApplyTags(std::vector<Tag::Tags> skillTags);
    std::weak_ptr<GameObject>  AddObjTag(Tag::Tags tag);
    bool HasTag(Tag::Tags tagToCheck);
    void ActivateTag(Tag::Tags tag); 

    GameObject* indicator; 

private:
  LifeBar* lifeBarDaughter;
  Rect daughterHitbox;

  int tagSpaceCount;

  std::vector<std::weak_ptr<GameObject>> daughtertags;//only used for destructor

  std::map<Tag::Tags, int> tagCountMap; // Map to track tag counts, separated from mothertags

  int hp;
  std::vector<Tag::Tags> tags;

};