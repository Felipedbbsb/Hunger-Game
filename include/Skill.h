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
#include "Music.h"

#define TEXT_TAGCOUNT2_SIZE 42

#define SKILL_SELECTED_OBJ "assets/img/UI/uiSkillSelected.png"
#define SKILL_SELECTED_OBJ_REWARD "assets/img/UI/uiSkillSelectedReward.png"

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
#define SPR_PocketSand "assets/img/UI/skills/PocketSand.png"

#define NS_HyperProtective "Hyper Protective"
#define I_HyperProtective "assets/img/UI/skills/readers/readerHyperProtective.png"
#define SPR_HyperProtective "assets/img/UI/skills/HyperProtective.png"

#define NS_RiskyManeuver "Risky Maneuver"
#define I_RiskyManeuver "assets/img/UI/skills/readers/readerRiskyManeuver.png"
#define SPR_RiskyManeuver "assets/img/UI/skill/RiskyManeuver.png"

#define NS_BattleHymn "Battle Hymn"
#define I_BattleHymn "assets/img/UI/skills/readers/readerBattleHymn.png"
#define SPR_BattleHymn "assets/img/UI/skills/BattleHymn.png"

#define NS_SwiftFooted "Swift Footed"
#define I_SwiftFooted "assets/img/UI/skills/readers/readerSwiftFooted.png"
#define SPR_SwiftFooted "assets/img/UI/skills/SwiftFooted.png"

#define NS_BlindedbyFear "Blinded by Fear"
#define I_BlindedbyFear "assets/img/UI/skills/readers/readerBlindedbyFear.png"
#define SPR_BlindedbyFear "assets/img/UI/skills/BlindedbyFear.png"

#define NS_Solitude "Solitude"
#define I_Solitude "assets/img/UI/skills/readers/readerSolitude.png"
#define SPR_Solitude "assets/img/UI/skills/Solitude.png"

#define NS_MaternalInstincts "Maternal Instincts"
#define I_MaternalInstincts "assets/img/UI/skills/readers/readerMaternalInstincts.png"
#define SPR_MaternalInstincts "assets/img/UI/skills/MaternalInstincts.png"

//=================Daughter=================

#define NS_NanaNanaNa "Nana Nana Na"
#define I_NanaNanaNa "assets/img/UI/skills/readers/readerNanaNanaNa.png"
#define SPR_NanaNanaNa "assets/img/UI/skills/NanaNanaNa.png"

#define NS_PommelStrike "Pommel Strike"
#define I_PommelStrike "assets/img/UI/skills/readers/readerPommelStrike.png"
#define SPR_PommelStrike "assets/img/UI/skills/PommelStrike.png"

#define NS_HnS "Hide and Seek"
#define I_HnS "assets/img/UI/skills/readers/readerHideandSeek.png"
#define SPR_HnS "assets/img/UI/skills/HideandSeek.png"

#define NS_TrickorTreat "Trick or Treat"
#define I_TrickorTreat "assets/img/UI/skills/readers/readerTrickorTreat.png"
#define SPR_TrickorTreat "assets/img/UI/uiSkillNormal.png"

#define NS_SweetTreats "Sweet Treats"
#define I_SweetTreats "assets/img/UI/skills/readers/readerSweetTreats.png"
#define SPR_SweetTreats "assets/img/UI/skill/SweetTreats.png"

#define NS_AroundtheRosie "Around the Rosie"
#define I_AroundtheRosie "assets/img/UI/skills/readers/readerAroundtheRosie.png"
#define SPR_AroundtheRosie "assets/img/UI/skills/AroudtheRosie.png"

#define NS_TagYoureIt "Tag! You are It"
#define I_TagYoureIt "assets/img/UI/skills/readers/readerTagYoureIt.png"
#define SPR_TagYoureIt "assets/img/UI/skills/TagYoureIt.png"

#define NS_RedLight "Red Light"
#define I_RedLight "assets/img/UI/skills/readers/readerRedLight.png"
#define SPR_RedLight "assets/img/UI/skills/RedLight.png"

#define NS_GreenLight "Green Light"
#define I_GreenLight "assets/img/UI/skills/readers/readerGreenLight.png"
#define SPR_GreenLight "assets/img/UI/skills/GreenLight.png"

#define NS_TemperTantrum "Temper Tantrum"
#define I_TemperTantrum "assets/img/UI/skills/readers/readerTemperTantrum.png"
#define SPR_TemperTantrum "assets/img/UI/skills/TemperTantrum.png"

#define NS_DesperateFlailing "Desperate Flailing"
#define I_DesperateFlailing "assets/img/UI/skills/readers/readerDesperateFlailing.png"
#define SPR_DesperateFlailing "assets/img/UI/skills/DesperateFlailing.png"



//=================Djinn=================
#define NS_InstantRegret "Instant Regret"
#define I_InstantRegret "assets/img/UI/skills/readers/readerInstantRegret.png"
#define SPR_InstantRegret "assets/img/UI/skills/InstantRegret.png"

#define NS_AMillionStabs "A Million Stabs"
#define I_AMillionStabs "assets/img/UI/skills/readers/readerAMillionStabs.png"
#define SPR_AMillionStabs "assets/img/UI/skills/AMillionStabs.png"

#define NS_DanseMacabre "Danse Macabre"
#define I_DanseMacabre "assets/img/UI/skills/readers/readerDanseMacabre.png"
#define SPR_DanseMacabre "assets/img/UI/skills/DanseMacabre.png"

#define NS_Hellfire "Hellfire"
#define I_Hellfire "assets/img/UI/skills/readers/readerHellfire.png"
#define SPR_Hellfire "assets/img/UI/skills/Hellfire.png"

#define NS_Gravedigger "Gravedigger"
#define I_Gravedigger "assets/img/UI/skills/readers/readerGravedigger.png"
#define SPR_Gravedigger "assets/img/UI/skills/Gravedigger.png"

#define NS_Combustion "Combustion"
#define I_Combustion "assets/img/UI/skills/readers/readerCombustion.png"
#define SPR_Combustion "assets/img/UI/skills/Combustion.png"

#define NS_DevilsTears "Devil's Tears"
#define I_DevilsTears "assets/img/UI/skills/readers/readerDevilsTears.png"
#define SPR_DevilsTears "assets/img/UI/skills/DevilsTears.png"

#define NS_FinalSacrifice "Final Sacrifice"
#define I_FinalSacrifice "assets/img/UI/skills/readers/readerFinalSacrifice.png"
#define SPR_FinalSacrifice "assets/img/UI/skills/FinalSacrifice.png"

#define NS_TormentedSoul "Tormented Soul"
#define I_TormentedSoul "assets/img/UI/skills/readers/readerTormentedSoul.png"
#define SPR_TormentedSoul "assets/img/UI/skills/TormentedSoul.png"

#define NS_RainingBlood "Raining Blood"
#define I_RainingBlood "assets/img/UI/skills/readers/readerRainingBlood.png"
#define SPR_RainingBlood "assets/img/UI/skills/RainingBlood.png"

#define NS_MasterofPuppets "Master of Puppets"
#define I_MasterofPuppets "assets/img/UI/skills/readers/readerMasterofPuppets.png"
#define SPR_MasterofPuppets "assets/img/UI/uiSkillDjinn.png"

#define NS_IronMaiden "Iron Maiden"
#define I_IronMaiden "assets/img/UI/skills/readers/readerIronMaiden.png"
#define SPR_IronMaiden "assets/img/UI/uiSkillDjinn.png"

#define NS_DeafeningWhisper "Deafening Whisper"
#define I_DeafeningWhisper "assets/img/UI/skills/readers/readerDeafeningWhisper.png"
#define SPR_DeafeningWhisper "assets/img/UI/skills/DeafeningWhisper.png"

#define NS_IntotheVoid "Into the Void"
#define I_IntotheVoid "assets/img/UI/skills/readers/readerIntotheVoid.png"
#define SPR_IntotheVoid "assets/img/UI/skills/IntotheVoid.png"

#define NS_FearoftheDark "Fear of the Dark"
#define I_FearoftheDark "assets/img/UI/skills/readers/readerFearoftheDark.png"
#define SPR_FearoftheDark "assets/img/UI/skills/FearoftheDark.png"

#define NS_Elimination "Elimination"
#define I_Elimination "assets/img/UI/skills/readers/readerElimination.png"
#define SPR_Elimination "assets/img/UI/skills/Elimination.png"


//=================Enemies=================
#define NS_Generic "Attack1"
#define I_Generic "assets/img/UI/uiSkillDjinn.png"
#define SPR_Generic "assets/img/UI/uiSkillDjinn.png"

//LOCKED
#define NS_LOCKED "LOCKED"
#define I_LOCKED "assets/img/UI/skills/readers/uiReaderLocked1.png"
#define SPR_LOCKED "assets/img/UI/skills/Locked.png"

#define NS_LOCKED2 "LOCKED"
#define I_LOCKED2 "assets/img/UI/skills/readers/uiReaderLocked2.png"
#define SPR_LOCKED2 "assets/img/UI/skills/Locked.png"

#define NS_LOCKED3 "LOCKED"
#define I_LOCKED3 "assets/img/UI/skills/readers/uiReaderLocked3.png"
#define SPR_LOCKED3 "assets/img/UI/skills/Locked.png"

//EMPTY
#define NS_EMPTY "EMPTY"
#define I_EMPTY "EMPTY"
#define SPR_EMPTY "assets/img/UI/skills/Empty.png"


//-------------------------------------------------------------------
#define SKILL_READER_SPRITE "assets/img/UI/uiSkillReader.png"

#define SKILL_CLICK_COOLDOWN 0.1
//===================================================
#define SKILL_SELECTION "assets/audio/sfxSelection.mp3"

#define SKILL_SELECTION_CONFIRMED "assets/audio/sfxSelectionConfirm.mp3"

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
            
            
            PocketSand,
            NanaNanaNa,
            TrickorTreat,
            HnS,
            SweetTreats,
            AroundtheRosie,
            TagYoureIt,
            RedLight,
            GreenLight,
            TemperTantrum,
            DesperateFlailing,

            InstantRegret,
            AMillionStabs,
            DanseMacabre,
            Hellfire,
            Gravedigger,
            Combustion,
            DevilsTears,
            FinalSacrifice,
            TormentedSoul,
            RainingBlood,
            MasterofPuppets,
            IronMaiden,
            DeafeningWhisper,
            IntotheVoid,
            FearoftheDark,
            Elimination,


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
            
            E_Cut,
            E_Whip,
            E_Evil_Chant,
            E_Shiv,
            E_Tentacle_Strike,
            E_Bite, 
            E_Beak,
            E_Unnerving_Presence,
            E_Guttural_Scream,
            E_Infernal_Scream,
            E_Infernal_Skull,
            E_Enrage,
            E_Impale,
            E_Freezing_Stare,
            E_Empower,
            E_Take_Soul,
            E_Bubble_Shield,
            E_Tongue_Strike,
            E_Toxic_Sludge,
            E_Lick,
            E_Digest,
            E_Inflate,

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
            DJINN,
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
        void BlankTagCount(bool isBlank);
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
        GameObject* skillSelected;
        GameObject* selectSFX;
};
 