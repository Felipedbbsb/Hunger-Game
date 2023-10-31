#include "GameObject.h"
#include "Sprite.h"
#include "Game.h"
#include "Enemies.h"
#include "Component.h"

#define MOTHER_SPRITE_ATK "assets/img/mom/momMeleeAttack.png"
#define MOTHER_SPRITE_DFS "assets/img/mom/momDamage.png"

#define DAUGHTER_SPRITE_ATK "assets/img/daughter/daughterMeleeAttack.png"
#define DAUGHTER_SPRITE_DFS "assets/img/daughter/daughterDamage.png"



class InteractionObject : public Component {
    public:
        InteractionObject(GameObject &associated, 
                            Skill::TargetType targetType, 
                            Enemies::EnemyId enemyId, 
                            bool isAttacking);

        ~InteractionObject();
        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);

        // Map to store enemy information

    private:
        Skill::TargetType targetType;
        Enemies::EnemyId enemyId;
        std::string iconPath;

        bool isAttacking;
};  