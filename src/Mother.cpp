#include "Mother.h"
#include "GameData.h"
#include "Enemies.h"
#include "Camera.h"
#include "Game.h"


#ifdef DEBUG

#include <SDL2/SDL.h>
#endif //DEBUG

// speed já está sendo inicializado pelo construtor de Vec2
Mother::Mother(GameObject &associated) : 
Component::Component(associated),
indicator(nullptr),
intention(nullptr),
lifeBarMother(nullptr),
tagSpaceCount(0){

    hp = 50;
    tags = {};
}

void Mother::Start() 
{
    Sprite *mother_spr = new Sprite(associated, MOTHER_SPRITE);
    associated.AddComponent((std::shared_ptr<Sprite>)mother_spr); 
    associated.box.y -= associated.box.h;

    //===================================Hitbox==================================
    motherHitbox = Rect(associated.box.x, associated.box.y, 130, associated.box.h);

    associated.box.x -= (associated.box.w - motherHitbox.w )/2;

    //==================================LifeBar====================================
    lifeBarMother = new LifeBar(associated, hp, hp, motherHitbox.w, motherHitbox.x); //width from hitbox
    associated.AddComponent(std::shared_ptr<LifeBar>(lifeBarMother));

    //If enemies starts with tags
    ApplyTags(tags);

    lifeBarMother->SetCurrentHP(hp); 
}

Mother::~Mother()
{
    for (int i = mothertags.size() - 1; i >= 0; i--) { //remove enemies tags
            mothertags.erase(mothertags.begin() + i);
    }

    DeleteIndicator();
    DeleteIntention();
} 

void Mother::Update(float dt)
{   
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());

    auto selectedSkill = Skill::selectedSkill;
    auto selectedSkillEnemy = Skill::selectedSkillEnemy;
    auto skillBack = Skill::skillBackToMother;



    // Check if the enemy's HP is zero or below and request deletion
    //if (hp <= 0) {
    //    DeleteEnemyIndicator();
    //    associated.RequestDelete();
    //    return; // Early exit if the enemy is no longer alive
    //} 




    //ENEMY TURN
    if(GameData::playerTurn == false){
        DeleteIntention();
        DeleteIndicator();

        //=============================Targeted skill sector=============================
        //Sector to manipulate interections involving mother being attacked
        if (selectedSkillEnemy){
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkillEnemy->GetId()];
            if((tempSkillInfo.attackType == Skill::AttackType::ATTACK_INDIVIDUAL || tempSkillInfo.attackType == Skill::AttackType::DEBUFF_INDIVIDUAL)
            && Skill::playerTargetType == Skill::MOTHER){
                ApplySkillToMother(tempSkillInfo.damage, tempSkillInfo.tags);
                Skill::selectedSkillEnemy = nullptr;
                Enemies::enemyAttacking = false;
            }
        }
    }

    //PLAYER TURN
    else{

        //--------------Intention manager------------------
        //Shows who wants to attack
        if (selectedSkill) {
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];
            if(tempSkillInfo.targetTypeAttacker == Skill::TargetType::MOTHER){
                if(intention == nullptr){
                    CreateIntention();
                }                      
            }else{
                DeleteIntention();
            }
        
        }else{
            DeleteIntention(); 
        }


        //=============================Skill buff sector==============================
        if (selectedSkill) {// Check if a skill is selected
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[selectedSkill->GetId()];
            if(tempSkillInfo.attackType == Skill::AttackType::BUFF_INDIVIDUAL || tempSkillInfo.attackType == Skill::AttackType::BUFF_ALL ){
                if(tempSkillInfo.targetTypeAttacker != Skill::TargetType::MOTHER){ //With its mother who caste probably the buff is for the daughter
                    if (indicator == nullptr){
                        CreateIndicator(); // Create an indicator if it doesn't exist
                    }

                    // Check if the mouse is over the enemy and left mouse button is pressed
                    //TODO case of being buff_all
                    if (motherHitbox.Contains(mousePos.x, mousePos.y) && inputManager.MousePress(LEFT_MOUSE_BUTTON)) {
                        AP::apCount -= tempSkillInfo.apCost;
                        ApplySkillToMother(tempSkillInfo.damage, tempSkillInfo.tags);
                        selectedSkill->Deselect();  
                    } 
                }
                else{
                    DeleteIndicator();
                }
            }
            else{
                DeleteIndicator();// Delete the  indicator if it exists skill type not buff
            }                               
        }else {
            DeleteIndicator();// Delete the  indicator if it exists
        }    

        
        //=============================Skill back sector=================================
        //Sector to manipulate interections involving enemies being attacked

        if (skillBack != nullptr) {
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[skillBack->GetId()];
            ApplySkillToMother(tempSkillInfo.damageBack, tempSkillInfo.tagsBack);
            skillBack->DeselectBack(tempSkillInfo.targetTypeBack);
        }
    }    
}


void Mother::CreateIndicator() {
    indicator = new GameObject(motherHitbox.x, motherHitbox.y + motherHitbox.h);
    Sprite* indicator_spr = new Sprite(*indicator, MOTHER_INDICATOR_SPRITE);

    // Scale the enemy indicator
    float percentageEnemyWidth = motherHitbox.w / indicator->box.w;
    indicator_spr->SetScale(percentageEnemyWidth, 1);
    indicator->AddComponent(std::make_shared<Sprite>(*indicator_spr));
    Game::GetInstance().GetCurrentState().AddObject(indicator);
}

void Mother::DeleteIndicator() {
    if (indicator != nullptr) {
        indicator->RequestDelete();
        indicator = nullptr;
    }
}

void Mother::CreateIntention() {
    intention = new GameObject(motherHitbox.x+ motherHitbox.w/2, motherHitbox.y);
    Sprite* intention_spr = new Sprite(*intention, MOTHER_INTENTON_SPRITE);
    intention->AddComponent(std::make_shared<Sprite>(*intention_spr));
    intention->box.x -= intention->box.w/2;
    intention->box.y -= intention->box.h/2;
    Game::GetInstance().GetCurrentState().AddObject(intention);
}

void Mother::DeleteIntention() {
    if (intention != nullptr) {
        intention->RequestDelete();
        intention = nullptr;
    }
}

void Mother::ApplySkillToMother(int damage, std::vector<Tag::Tags> tags) {
        float tagMultiplier = 1; //multiplier without tags
        if(damage > 0){
            if (HasTag(Tag::Tags::RESILIENCE)){
                ActivateTag(Tag::Tags::RESILIENCE);
                tagMultiplier -= 0.5; 
            }
            if (HasTag(Tag::Tags::VULNERABLE)){
                ActivateTag(Tag::Tags::VULNERABLE);
                tagMultiplier += 0.5; 
            }
        }
        hp -= damage * tagMultiplier;
        ApplyTags(tags);
        lifeBarMother->SetCurrentHP(hp);  // Update the enemy's HP bar
 
}
 
void Mother::ApplyTags(std::vector<Tag::Tags> skillTags) {
    for (auto& tag : skillTags) {
        ActivateTag(tag);
        if (tagCountMap.find(tag) != tagCountMap.end()) {
            // The tag already exists, increment the counter
            tagCountMap[tag]++;

            // Iterate over the list of weak_ptr to the tag GameObjects
            for (auto& weak_tag : mothertags) {
                auto tagGameObject = weak_tag.lock();  // Get the GameObject
                if (tagGameObject) {
                    // Try to retrieve the "Tag" component
                    auto tagComponent = tagGameObject->GetComponent("Tag");
                    auto tagComponentPtr = std::dynamic_pointer_cast<Tag>(tagComponent);
                    if (tagComponentPtr) {
                        // Now, you can access the "tag" property of the "Tag" component
                        if (tagComponentPtr->GetTag() == tag) {
                            tagComponentPtr->UpdateQuantity(tagCountMap[tag]);
                        }
                    }
                }
            }
        } else {
            // The tag doesn't exist in the map, add it with a counter of 1
            tagCountMap[tag] = 1;
            tags.push_back(tag);
            auto go_tag = AddObjTag(tag);
            

        }
    }
}

void Mother::ActivateTag(Tag::Tags tag){
    for (auto& weak_tag : mothertags) {
        auto tagGameObject = weak_tag.lock();  // Get the GameObject
        if (tagGameObject) {
            // Try to retrieve the "Tag" component 
            auto tagComponent = tagGameObject->GetComponent("Tag");
            auto tagComponentPtr = std::dynamic_pointer_cast<Tag>(tagComponent);
            if (tagComponentPtr) {
                // Now, you can access the "tag" property of the "Tag" component
                if (tagComponentPtr->GetTag() == tag) {
                    tagComponentPtr->AcivateTag(tagComponentPtr->GetTagSprite(tag));
                }
            }
        }
    }
}


std::weak_ptr<GameObject>  Mother::AddObjTag(Tag::Tags tag){ 
    std::weak_ptr<GameObject> weak_enemy = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);

    GameObject* tagObject = new GameObject();
    Tag* tag_behaviour = new Tag(*tagObject, tag, weak_enemy, 1);
    tagObject->AddComponent(std::shared_ptr<Tag>(tag_behaviour));

    tagObject->box.x = motherHitbox.x + TAGS_SPACING_X * tagSpaceCount;
    tagObject->box.y = motherHitbox.y + motherHitbox.h + TAGS_SPACING_Y;
    std::weak_ptr<GameObject> go_tag = Game::GetInstance().GetCurrentState().AddObject(tagObject);

    tagSpaceCount += 1;
    mothertags.push_back(go_tag);  

    return go_tag;
}

bool Mother::HasTag(Tag::Tags tagToCheck) {
    // Go through the enemy's tag list and check if the desired tag is present. 
    for (const auto& tag : tags) {
        if (tag == tagToCheck) {
            return true; // tag is present
        }
    }
    return false; // tag isnt present.
}



void Mother::Render()
{
#ifdef DEBUG
    Vec2 center(motherHitbox.GetCenter());
    SDL_Point points[5];

    Vec2 point = (Vec2(motherHitbox.x, motherHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = (Vec2(motherHitbox.x + motherHitbox.w, motherHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[1] = {(int)point.x, (int)point.y};

    point = (Vec2(motherHitbox.x + motherHitbox.w, motherHitbox.y + motherHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[2] = {(int)point.x, (int)point.y};

    point = (Vec2(motherHitbox.x, motherHitbox.y + motherHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

bool Mother::Is(std::string type){
    return (type == "Mother");
}

