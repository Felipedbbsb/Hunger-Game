#include "GameObject.h"
#include "Sprite.h"
#include "Game.h"
#include "Enemies.h"
#include "Component.h"

#define MOTHER_SPRITE_ATK "assets/img/mom/momMeleeAttack.png"
#define MOTHER_SPRITE_DFS "assets/img/mom/momDamage.png"
#define MOTHER_SPRITE_BUFF "assets/img/mom/momDamage.png"

#define DAUGHTER_SPRITE_ATK "assets/img/daughter/daughterMeleeAttack.png"
#define DAUGHTER_SPRITE_DFS "assets/img/daughter/daughterDamage.png"
#define DAUGHTER_SPRITE_BUFF "assets/img/daughter/daughterBuff.png"

#define ATK_SPRITE "assets/img/UI/effectDamage.png"
#define DEBUFF_SPRITE "assets/img/UI/effectDebuff.png" 
#define BUFF_SPRITE "assets/img/UI/effectBuff.png"

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

        void CreateEffect(std::string TypeEffect);
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
};  