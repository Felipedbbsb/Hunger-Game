#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "GameObject.h"
#include "Component.h"
#include "Sound.h"
#include "Sprite.h"
#include "Time.h"
#include "Tag.h" 
#include "AP.h" 

#define TEXT_TAGCOUNT2_SIZE 42





//==================================================================
//(Not "\n\n")  - (Yes "\n \n" )
#define SKILL1_SPRITE "assets/img/UI/uiSkillDjinn.png"
#define NAME_SKILL1 "     Ataque pika  "
#define INFO_SKILL1 "5 damage all \n Apply Vulenerable "

#define SKILL2_SPRITE "assets/img/UI/uiSkillNormal.png"
#define NAME_SKILL2 "     Ataque paia  "
#define INFO_SKILL2 "20 damage\n Apply Resilience"

#define SKILL3_SPRITE "assets/img/UI/uiSkillDjinn.png"
#define NAME_SKILL3 "     Defesa top  "
#define INFO_SKILL3 "5 damage \nApply Dodge"

#define SKILL4_SPRITE "assets/img/UI/uiSkillNormal.png"
#define NAME_SKILL4 "     Defesa pika  "
#define INFO_SKILL4 "5 damage"

//NS = Name Skill
//I  = Info
//SPR = Sprite

//=================MOTHER SKILLS=================
#define NS_Helmbreaker "Helmbreaker"
#define I_Helmbreaker "assets/img/UI/skills/readers/readerHelmbreaker.png"
#define SPR_Helmbreaker "assets/img/UI/skills/Helmbreaker.png"

#define NS_Rockabye "Rockabye"
#define I_Rockabye "assets/img/UI/skills/readers/readerRockabye.png"
#define SPR_Rockabye "assets/img/UI/skills/Rockabye.png"

#define NS_Stinger "Stinger"
#define I_Stinger "assets/img/UI/skills/readers/readerStinger.png"
#define SPR_Stinger "assets/img/UI/skills/Stinger.png"

#define NS_RecklessSlash "Reckless Slash"
#define I_RecklessSlash "assets/img/UI/skills/readers/readerRecklessSlash.png"
#define SPR_RecklessSlash "assets/img/UI/uiSkillNormal.png"

#define NS_CautiousStrike "Cautious Strike"
#define I_CautiousStrike "assets/img/UI/skills/readers/readerCautiousStrike.png"
#define SPR_CautiousStrike "assets/img/UI/uiSkillNormal.png"

#define NS_MotherlyLove "Motherly Love"
#define I_MotherlyLove "assets/img/UI/skills/readers/readerMotherlyLove.png"
#define SPR_MotherlyLove "assets/img/UI/uiSkillNormal.png"


#define NS_PocketSand "Pocket Sand"
#define I_PocketSand "assets/img/UI/skills/readers/readerPocketSand.png"
#define SPR_PocketSand "assets/img/UI/uiSkillNormal.png"

#define NS_HyperProtective "Hyper Protective"
#define I_HyperProtective "assets/img/UI/skills/readers/readerHyperProtective.png"
#define SPR_HyperProtective "assets/img/UI/uiSkillNormal.png"

#define NS_RiskyManeuver "Risky Maneuver"
#define I_RiskyManeuver "assets/img/UI/skills/readers/readerRiskyManeuver.png"
#define SPR_RiskyManeuver "assets/img/UI/uiSkillNormal.png"

#define NS_BattleHymn "Battle Hymn"
#define I_BattleHymn "assets/img/UI/skills/readers/readerBattleHymn.png"
#define SPR_BattleHymn "assets/img/UI/uiSkillNormal.png"

#define NS_SwiftFooted "Swift Footed"
#define I_SwiftFooted "assets/img/UI/skills/readers/readerSwiftFooted.png"
#define SPR_SwiftFooted "assets/img/UI/uiSkillNormal.png"

#define NS_BlindedbyFear "Blinded by Fear"
#define I_BlindedbyFear "assets/img/UI/skills/readers/readerBlindedbyFear.png"
#define SPR_BlindedbyFear "assets/img/UI/uiSkillNormal.png"

#define NS_Solitude "Solitude"
#define I_Solitude "assets/img/UI/skills/readers/readerSolitude.png"
#define SPR_Solitude "assets/img/UI/uiSkillNormal.png"

#define NS_MaternalInstincts "Maternal Instincts"
#define I_MaternalInstincts "assets/img/UI/skills/readers/readerMaternalInstincts.png"
#define SPR_MaternalInstincts "assets/img/UI/uiSkillNormal.png"

//=================Daughter=================
#define NS_HnS "Hide and Seek"
#define I_HnS "assets/img/UI/skills/readers/readerHideandSeek.png"
#define SPR_HnS "assets/img/UI/skills/HideandSeek.png"

#define NS_NanaNanaNa "Nana Nana Na"
#define I_NanaNanaNa "assets/img/UI/skills/readers/readerNanaNanaNa.png"
#define SPR_NanaNanaNa "assets/img/UI/uiSkillNormal.png"

#define NS_PommelStrike "Pommel Strike"
#define I_PommelStrike "assets/img/UI/skills/readers/readerPommelStrike.png"
#define SPR_PommelStrike "assets/img/UI/uiSkillNormal.png"


//=================Djinn=================
#define NS_InstantRegret "Instant Regret"
#define I_InstantRegret "assets/img/UI/skills/readers/readerInstantRegret.png"
#define SPR_InstantRegret "assets/img/UI/skills/InstantRegret.png"

//=================Enemies=================
#define NS_Generic "Attack1"
#define I_Generic "assets/img/UI/uiSkillDjinn.png"
#define SPR_Generic "assets/img/UI/uiSkillDjinn.png"

//LOCKED
#define NS_LOCKED "LOCKED"
#define I_LOCKED "Desbloqueado com Np1"
#define SPR_LOCKED "assets/img/UI/skills/Locked.png"

#define NS_LOCKED2 "LOCKED"
#define I_LOCKED2 "Desbloqueado com Np2"
#define SPR_LOCKED2 "assets/img/UI/skills/Locked.png"

#define NS_LOCKED3 "LOCKED"
#define I_LOCKED3 "Desbloqueado com Np3"
#define SPR_LOCKED3 "assets/img/UI/skills/Locked.png"

//EMPTY
#define NS_EMPTY "EMPTY"
#define I_EMPTY "EMPTY"
#define SPR_EMPTY "assets/img/UI/skills/Empty.png"


//-------------------------------------------------------------------
#define SKILL_READER_SPRITE "assets/img/UI/uiSkillReader.png"

#define SKILL_CLICK_COOLDOWN 0.1
//===================================================


class Skill : public Component {
    public:
        enum SkillId {
            SKILL1,
            SKILL2,
            SKILL3,
            SKILL4,
            
            Helmbreaker,
            Rockabye,
            Stinger,
            RecklessSlash,
            CautiousStrike,
            MotherlyLove,
            PommelStrike,
            HyperProtective,
            RiskyManeuver,
            BattleHymn,
            SwiftFooted,
            BlindedbyFear,
            Solitude,
            MaternalInstincts,

            HnS,
            PocketSand,
            NanaNanaNa,

            InstantRegret,

            LOCKED1,
            LOCKED2,
            LOCKED3,

            EMPTY,

            E1_Skill1,
            E1_Skill2,
            E1_Skill3,

            E2_Skill1,
            E2_Skill2,
            E2_Skill3,

            E3_Skill1,
            E3_Skill2,
            
            

            INVALID_SKILL
        };
        
        enum AttackType {
            ATTACK_INDIVIDUAL,
            ATTACK_ALL,

            DEFENSE_INDIVIDUAL,
            DEFENSE_ALL,

            BUFF_INDIVIDUAL,
            BUFF_ALL,
            
            DEBUFF_INDIVIDUAL,
            DEBUFF_ALL,

            NONE
        };

        enum TargetType {
            MOTHER,
            DAUGHTER, 
            IRR //irrelevant
        };

        enum StateProtected {
            PROTECTED,
            EXPOSED, 
            NOCHANGES //irrelevant
        };

        struct SkillInfo {
            int apCost;
            StateProtected isProtected;
            int damage;
            std::vector<Tag::Tags> tags;
            int damageBack;
            std::vector<Tag::Tags> tagsBack;
            std::string name;
            std::string info;
            std::string iconPath;
            AttackType attackType;
            TargetType targetTypeAttacker; //who is attacking
            AttackType attackTypeBack;
            TargetType targetTypeBack;
        };
        
 

        Skill(GameObject& associated, SkillId id, AP* ap, bool createJewel = true);
        ~Skill();

        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);

        SkillId GetId();

        static Skill* skillFromReward;
        static Skill* skillToReward;

        static Skill* selectedSkill;

        static Skill* selectedSkillEnemy;
        
        static Skill* skillBackToMother;

        static Skill* skillBackToDaughter;

        static std::pair<bool, bool> HasTagRampageOrWeak;

        static TargetType playerTargetType;

        void Deselect();
        void DeselectBack(TargetType targetTypeBack);
        void SkillBack(TargetType targetTypeBack);

        static std::map<SkillId, SkillInfo> skillInfoMap; 
        static std::vector<Skill::SkillId> skillArray;
        static std::vector<std::weak_ptr<GameObject>> skillArrayObj;

        static void InitializeSkillInfoMap();
        static void InitializeSkills();

        void CreateTagCount();

        static void AddSkill(Skill::SkillId id, Skill::SkillId skillIdToChange = Skill::EMPTY);

    private:
        SkillId id;
        Timer skillClickTimer;
        GameObject* readerSkill;
        std::string textSkill = "";
        AP* apInstance;
        GameObject* jewelObj;
        GameObject* tagCount;
        bool toggleJewel;
        bool createJewel;
};
 