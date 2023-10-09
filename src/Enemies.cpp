#include "Enemies.h"


Enemies::Enemies(GameObject &associated, EnemyId id) 
: Component::Component(associated),
id(id)
{
    
}

void Enemies::Start() 
{
     std::string spriteEnemy; // Declare spriteEnemy 

    switch (id) {
        case EnemyId::ENEMY1: 
            spriteEnemy = ENEMEY1_SPRITE;
            //textEnemy = TEXT_Enemy1; 
            break; // Use "break" para sair do switch após cada case
 
        case EnemyId::ENEMY2: 
            spriteEnemy = ENEMEY2_SPRITE;
            //textEnemy = TEXT_Enemy2;
            break; // Use "break" para sair do switch após cada case
        case EnemyId::ENEMY3:  
            spriteEnemy = ENEMEY3_SPRITE;
            //textEnemy = TEXT_Enemy3;
            break; // Use "break" para sair do switch após cada case
 
        case EnemyId::ENEMY4: 
            spriteEnemy = ENEMEY4_SPRITE;
            //textEnemy = TEXT_Enemy4;
            break; // Use "break" para sair do switch após cada case    
    }
 
    // Add  sprite
    Sprite *enemies_spr = new Sprite(associated, spriteEnemy);
    enemies_spr->SetScale(0.15, 0.15);
    associated.AddComponent((std::shared_ptr<Sprite>)enemies_spr); 
}

Enemies::~Enemies()
{
     
}

void Enemies::Update(float dt)
{   
    
}

void Enemies::Render()
{
}

Enemies::EnemyId Enemies::GetId(){
    return id; 
}

bool Enemies::Is(std::string type){
    return (type == "Enemies");
}

