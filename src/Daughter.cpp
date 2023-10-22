#include "Daughter.h"
#include "GameData.h"

#ifdef DEBUG
#include "Camera.h"
#include "Game.h"

#include <SDL2/SDL.h>
#endif //DEBUG

Daughter::Daughter(GameObject &associated) : 
Component::Component(associated),
lifeBarDaughter(nullptr){
    // Add  sprite
    hp = 25;
    tags = {Tag::Tags::DODGE};
}

void Daughter::Start() 
{
    Sprite *daughter_spr = new Sprite(associated, DAUGHTER_SPRITE);
    associated.AddComponent((std::shared_ptr<Sprite>)daughter_spr); 
    associated.box.y -= associated.box.h;

    //===================================Hitbox==================================
    daughterHitbox = Rect(associated.box.x, associated.box.y, 130, associated.box.h);

    associated.box.x -= (associated.box.w - daughterHitbox.w )/2;

    //==================================LifeBar====================================
    lifeBarDaughter = new LifeBar(associated, hp, hp, daughterHitbox.w, daughterHitbox.x); //width from hitbox
    associated.AddComponent(std::shared_ptr<LifeBar>(lifeBarDaughter));

    //If enemies starts with tags
    ApplyTags(tags);

    lifeBarDaughter->SetCurrentHP(hp); 

}
 
Daughter::~Daughter()
{
    for (int i = daughtertags.size() - 1; i >= 0; i--) { //remove enemies tags
            daughtertags.erase(daughtertags.begin() + i);
    }
    
}

void Daughter::Update(float dt)
{   
    auto& inputManager = InputManager::GetInstance();
    Vec2 mousePos(inputManager.GetMouseX(), inputManager.GetMouseY());

    auto skillBack = Skill::skillBackToDaughter;



    // Check if the enemy's HP is zero or below and request deletion
    //if (hp <= 0) {
    //    DeleteEnemyIndicator();
    //    associated.RequestDelete();
    //    return; // Early exit if the enemy is no longer alive
    //} 

    //ENEMY TURN
    if(GameData::playerTurn == false){
        //=============================Attacked skill sector=============================
        //Sector to manipulate interections involving daughter being attacked


        //=============================Skill defense sector==============================
        //TODO SKILL BUFF DEFENSE
    }

    //PLAYER TURN
    else{
        //=============================Skill back sector=================================
        //Sector to manipulate interections involving enemies being attacked

        if (skillBack != nullptr) {
            Skill::SkillInfo tempSkillInfo = Skill::skillInfoMap[skillBack->GetId()];
            ApllySkillToDaughter(tempSkillInfo.damageBack, tempSkillInfo.tagsBack);
            skillBack->DeselectBack(tempSkillInfo.targetTypeBack);
        }
    }    
}

void Daughter::ApllySkillToDaughter(int damage, std::vector<Tag::Tags> tags) {
        float tagMultiplier = 1; //multiplier without tags
        if (HasTag(Tag::Tags::RESILIENCE)){
            ActivateTag(Tag::Tags::RESILIENCE);
            tagMultiplier -= 0.5; 
        }
        if (HasTag(Tag::Tags::VULNERABLE)){
            ActivateTag(Tag::Tags::VULNERABLE);
            tagMultiplier += 0.5; 
        }

        hp -= damage * tagMultiplier;
        ApplyTags(tags);
        lifeBarDaughter->SetCurrentHP(hp);  // Update the enemy's HP bar
 
}
 
void Daughter::ApplyTags(std::vector<Tag::Tags> skillTags) {
    for (auto& tag : skillTags) {
        if (tagCountMap.find(tag) != tagCountMap.end()) {
            // The tag already exists, increment the counter
            tagCountMap[tag]++;

            // Iterate over the list of weak_ptr to the tag GameObjects
            for (auto& weak_tag : daughtertags) {
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

void Daughter::ActivateTag(Tag::Tags tag){
    for (auto& weak_tag : daughtertags) {
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


std::weak_ptr<GameObject>  Daughter::AddObjTag(Tag::Tags tag){ 
    std::weak_ptr<GameObject> weak_enemy = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);

    GameObject* tagObject = new GameObject();
    Tag* tag_behaviour = new Tag(*tagObject, tag, weak_enemy, 1);
    tagObject->AddComponent(std::shared_ptr<Tag>(tag_behaviour));

    tagObject->box.x = daughterHitbox.x + TAGS_SPACING_X * tagSpaceCount;
    tagObject->box.y = daughterHitbox.y + daughterHitbox.h + TAGS_SPACING_Y;
    std::weak_ptr<GameObject> go_tag = Game::GetInstance().GetCurrentState().AddObject(tagObject);

    tagSpaceCount += 1;
    daughtertags.push_back(go_tag);  

    return go_tag;
}

bool Daughter::HasTag(Tag::Tags tagToCheck) {
    // Go through the enemy's tag list and check if the desired tag is present. 
    for (const auto& tag : tags) {
        if (tag == tagToCheck) {
            return true; // tag is present
        }
    }
    return false; // tag isnt present.
}



void Daughter::Render()
{
#ifdef DEBUG
    Vec2 center(daughterHitbox.GetCenter());
    SDL_Point points[5];

    Vec2 point = (Vec2(daughterHitbox.x, daughterHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = (Vec2(daughterHitbox.x + daughterHitbox.w, daughterHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[1] = {(int)point.x, (int)point.y};

    point = (Vec2(daughterHitbox.x + daughterHitbox.w, daughterHitbox.y + daughterHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[2] = {(int)point.x, (int)point.y};

    point = (Vec2(daughterHitbox.x, daughterHitbox.y + daughterHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

bool Daughter::Is(std::string type){
    return (type == "Daughter");
}

