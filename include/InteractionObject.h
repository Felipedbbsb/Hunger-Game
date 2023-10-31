#include "GameObject.h"
#include "Sprite.h"
#include "Game.h"
#include "Enemies.h"
#include "Component.h"

class InteractionObject : public Component {
    public:
        InteractionObject(GameObject &associated, Skill::TargetType targetType, Enemies::EnemyId enemyId);
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
};  