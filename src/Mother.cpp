#include "Mother.h"
#include "GameData.h"

#ifdef DEBUG
#include "Camera.h"
#include "Game.h"

#include <SDL2/SDL.h>
#endif //DEBUG

// speed já está sendo inicializado pelo construtor de Vec2
Mother::Mother(GameObject &associated) : 
Component::Component(associated),
lifeBarMother(nullptr),
tagSpaceCount(0){

    hp = 50;
    tags = {Tag::Tags::VULNERABLE};
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
    
} 

void Mother::Update(float dt)
{   
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());

    auto skillBack = Skill::skillBackToMother;



    // Check if the enemy's HP is zero or below and request deletion
    //if (hp <= 0) {
    //    DeleteEnemyIndicator();
    //    associated.RequestDelete();
    //    return; // Early exit if the enemy is no longer alive
    //} 



    

    //ENEMY TURN
    if(GameData::playerTurn == false){
        //=============================Attacked skill sector=============================
        //Sector to manipulate interections involving mother being attacked


        //=============================Skill defense sector==============================
        //TODO SKILL BUFF DEFENSE
    }

    //PLAYER TURN
    else{
        //=============================Skill back sector=================================
        //Sector to manipulate interections involving enemies being attacked

        if (skillBack != nullptr) {
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[skillBack->GetId()];
            ApllySkillToMother(tempSkillInfo);
            skillBack->DeselectBack(tempSkillInfo.targetTypeBack);
        }
    }    
}


void Mother::ApllySkillToMother(Skill::SkillInfo& skillInfo) {
        float tagMultiplier = 1; //multiplier without tags
        if (HasTag(Tag::Tags::RESILIENCE)){
            ActivateTag(Tag::Tags::RESILIENCE);
            tagMultiplier -= 0.5; 
        }
        if (HasTag(Tag::Tags::VULNERABLE)){
            ActivateTag(Tag::Tags::VULNERABLE);
            tagMultiplier += 0.5; 
        }

        hp -= skillInfo.damage * tagMultiplier;
        ApplyTags(skillInfo.tags);
        lifeBarMother->SetCurrentHP(hp);  // Update the enemy's HP bar
 
}
 
void Mother::ApplyTags(std::vector<Tag::Tags> skillTags) {
    for (auto& tag : skillTags) {
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

