#include "CombatObject.h"
#include "InputManager.h"
#include "Skill.h" // Include the Skill header to access selectedSkill and SkillInfo
#include "LifeBar.h"
#include "Tag.h"
#include "Game.h"
#include "Enemies.h"
#include "AP.h" 
#include <algorithm> 

#ifdef DEBUG
#include "Camera.h"
#include "Game.h"

#include <SDL2/SDL.h>
#endif //DEBUG


 
CombatObject::CombatObject(GameObject& associated)
    : Component::Component(associated), 
    lifeBar(nullptr), 
    tagSpaceCount(0)
    {  


}
 
void CombatObject::Start() {
    //===================================Hitbox==================================
    enemyHitbox = Rect(associated.box.x, associated.box.y, 130, associated.box.h);

    associated.box.x -= (associated.box.w - enemyHitbox.w )/2;

    //==================================LifeBar====================================
    lifeBar = new LifeBar(associated, hp, hp, enemyHitbox.w, enemyHitbox.x); //width from hitbox
    associated.AddComponent(std::shared_ptr<LifeBar>(lifeBar));

    //If CombatObject starts with tags
    //ApplyTags(tags);

    //lifeBar->SetCurrentHP(hp); 

}

CombatObject::~CombatObject() { 
    for (int i = combatObjTags.size() - 1; i >= 0; i--) { //remove CombatObject tags
            combatObjTags.erase(combatObjTags.begin() + i);
    }

    if(HasTag(Tag::Tags::PROVOKE)){
        Enemies::provokedEnemies -= 1;
    }

}

void CombatObject::Update(float dt) {

}
 


void CombatObject::ApplySkill(Skill::SkillInfo& skillInfo) {
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
        lifeBar->SetCurrentHP(hp);  // Update the enemy's HP bar
     
}

void CombatObject::ApplyTags(std::vector<Tag::Tags> skillTags) {
    for (auto& tag : skillTags) {
        if (tagCountMap.find(tag) != tagCountMap.end()) {
            // The tag already exists, increment the counter
            tagCountMap[tag]++;

            // Iterate over the list of weak_ptr to the tag GameObjects
            for (auto& weak_tag : combatObjTags) {
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
            
            if(tag == Tag::Tags::PROVOKE){
                Enemies::provokedEnemies++;
            }
        }
    }
}
 

void CombatObject::ActivateTag(Tag::Tags tag){
    for (auto& weak_tag : combatObjTags) {
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


std::weak_ptr<GameObject>  CombatObject::AddObjTag(Tag::Tags tag){ 
    std::weak_ptr<GameObject> weak_enemy = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);

    GameObject* tagObject = new GameObject();
    Tag* tag_behaviour = new Tag(*tagObject, tag, weak_enemy, 1);
    tagObject->AddComponent(std::shared_ptr<Tag>(tag_behaviour));

    tagObject->box.x = enemyHitbox.x + TAGS_SPACING_X * tagSpaceCount;
    tagObject->box.y = enemyHitbox.y + enemyHitbox.h + TAGS_SPACING_Y;
    std::weak_ptr<GameObject> go_tag = Game::GetInstance().GetCurrentState().AddObject(tagObject);

    tagSpaceCount += 1;
    combatObjTags.push_back(go_tag);  

    return go_tag;
}

bool CombatObject::HasTag(Tag::Tags tagToCheck) {
    // Go through the enemy's tag list and check if the desired tag is present. 
    for (const auto& tag : tags) {
        if (tag == tagToCheck) {
            return true; // tag is present
        }
    }
    return false; // tag isnt present.
}

void CombatObject::Render() {
    #ifdef DEBUG
    Vec2 center(enemyHitbox.GetCenter());
    SDL_Point points[5];

    Vec2 point = (Vec2(enemyHitbox.x, enemyHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = (Vec2(enemyHitbox.x + enemyHitbox.w, enemyHitbox.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[1] = {(int)point.x, (int)point.y};

    point = (Vec2(enemyHitbox.x + enemyHitbox.w, enemyHitbox.y + enemyHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[2] = {(int)point.x, (int)point.y};

    point = (Vec2(enemyHitbox.x, enemyHitbox.y + enemyHitbox.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}



bool CombatObject::Is(std::string type) {
    return (type == "CombatObject"); 
}

