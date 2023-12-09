#include "GameObject.h"
#include "Sprite.h"
#include "Game.h"
#include "Enemies.h"
#include "Timer.h"
#include "Component.h"
#include "Papiro.h"

#define EFFECT_COOLDOWN 3

#define MOTHER_SPRITE_ATK "assets/img/mom/momMeleeAttack.png"
#define MOTHER_SPRITE_ATK_NP1 "assets/img/mom/momMeleeNP1.png"
#define MOTHER_SPRITE_ATK_NP2 "assets/img/mom/momMeleeNP2.png"

#define MOTHER_SPRITE_DFS "assets/img/mom/momDamage.png"
#define MOTHER_SPRITE_DFS_NP1 "assets/img/mom/momDamageNP1.png"
#define MOTHER_SPRITE_DFS_NP2 "assets/img/mom/momDamageNP2.png"

#define MOTHER_SPRITE_BUFF "assets/img/mom/momBuff.png"
#define MOTHER_SPRITE_BUFF_NP1 "assets/img/mom/momBuffNP1.png"
#define MOTHER_SPRITE_BUFF_NP2 "assets/img/mom/momBuffNP2.png"
 
#define DAUGHTER_SPRITE_ATK "assets/img/daughter/daughterMeleeAttack.png"
#define DAUGHTER_SPRITE_DFS "assets/img/daughter/daughterDamage.png"
#define DAUGHTER_SPRITE_BUFF "assets/img/daughter/daughterBuff.png"

#define ATK_SPRITE "assets/img/UI/effectDamage.png"
#define DEBUFF_SPRITE "assets/img/UI/effectDebuff.png" 
#define BUFF_SPRITE "assets/img/UI/effectBuff.png"

#define ATK_SPRITE_R "assets/img/UI/effectDamageR.png"
#define DEBUFF_SPRITE_R "assets/img/UI/effectDebuff.png" 
#define BUFF_SPRITE_R "assets/img/UI/effectBuffR.png"

class InteractionObject : public Component {
    public:
        InteractionObject(GameObject &associated, 
                            Skill::AttackType attackType,
                            Skill::TargetType targetType, 
                            Enemies::EnemyId enemyId, 
                            bool isAttacking);

        ~InteractionObject();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);

        void CreateEffect(std::string TypeEffect, bool isPlayer);
        void SetPos(int posX, int posY);

    private:
        Skill::TargetType targetType;
        Enemies::EnemyId enemyId;
        std::string iconPath;
        Skill::AttackType attackType;
        bool isAttacking;
        GameObject* effect;
        int posX;
        int posY;
        std::string typeEffect;
        Timer effectDuration;
};  