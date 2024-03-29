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
#include "Timer.h"

#define MOTHER_INDICATOR_SPRITE "assets/img/UI/uiIndicatorPlayer.png"

#define MOTHER_INTENTON_SPRITE "assets/img/UI/uiIntentionPlayer.png"

#define MOTHER_SPRITE "assets/img/mom/momIdle.png"
#define MOTHER_SPRITE_NP1 "assets/img/mom/momIdleNP1.png"
#define MOTHER_SPRITE_NP2 "assets/img/mom/momIdleNP2.png"
#define MOTHER_FC 14
#define MOTHER_FT 1.25

#define MOTHER_HP 60

#define MOTHER_DEATH_TIME 3

#define MOTHER_NP_SOUND "assets/audio/sfxNP.mp3"

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

    std::string GetSpriteMother();

    void RemoveOneTagAll();
    void RecreateTagUI();

    GameObject* indicator;   
    GameObject* intention; 

    static int hp;
    static int damageDjinn;
    static std::vector<Tag::Tags> tags;

    static bool activateRampage;
    static bool activateWeak;
    static std::weak_ptr<GameObject> motherInstance;
private:
    LifeBar* lifeBarMother;
    Rect motherHitbox;

    int tagSpaceCount;

    std::vector<std::weak_ptr<GameObject>> mothertags;//only used for destructor

    std::map<Tag::Tags, int> tagCountMap; // Map to track tag counts, separated from mothertags

    Timer deathTransitionTime;

    void IntentionAnimation(float dt);
    int ScaleIntention; //If 1 is growing, -1 the opposite
    void IndicatorAnimation(float dt);
    int ScaleIndicator; //If 1 is growing, -1 the opposite
};