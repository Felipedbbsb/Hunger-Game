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

#define MOTHER_SPRITE "assets/img/mom/momIdle.png"

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

    void ApllySkillToMother(Skill::SkillInfo& skillInfo);
    void ApplyTags(std::vector<Tag::Tags> skillTags);
    std::weak_ptr<GameObject>  AddObjTag(Tag::Tags tag);
    bool HasTag(Tag::Tags tagToCheck);
    void ActivateTag(Tag::Tags tag); 

private:
    LifeBar* lifeBarMother;
    Rect motherHitbox;

    int tagSpaceCount;

    std::vector<std::weak_ptr<GameObject>> mothertags;//only used for destructor

    std::map<Tag::Tags, int> tagCountMap; // Map to track tag counts, separated from mothertags

    int hp;
    std::vector<Tag::Tags> tags;

};