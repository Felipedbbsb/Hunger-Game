#include "UI.h"
#include "Game.h"
#include "Camera.h"
#include "AP.h"
#include "Protected.h"
#include "CameraFollower.h"
#include "Enemies.h"
#include "GameData.h"
#include "SkillSelection.h"
#include "CombatState.h"
#include "Mural.h"

// speed já está sendo inicializado pelo construtor de Vec2
// UI.h
GameObject* UI::uiGO = nullptr;

AP* UI::ap_behaviour = nullptr;

bool UI::nextActivated = false;

UI::UI(GameObject &associated)
    : Component(associated),
    nextArrow(nullptr),
    ScalenextArrow(1),
    CameraPosBuffer(0, 0){
    

    //AP
    GameObject* ap_UI = new GameObject(associated.box.x , associated.box.y);
        ap_behaviour = new AP(*ap_UI);
        new CameraFollower(*ap_UI);
        Game::GetInstance().GetCurrentState().AddObject(ap_UI);
 
    
    CreateSkillsGO(ap_behaviour);
} 
 


UI::~UI() 
{
    if(nextArrow != nullptr){
        nextArrow->RequestDelete();
        nextArrow = nullptr;
    }

    for (int i = Skill::skillArrayObj.size() - 1; i >= 0; i--) { //remove skills
            Skill::skillArrayObj.erase(Skill::skillArrayObj.begin() + i);
    }

    UI::ap_behaviour = nullptr;

    if(UI::uiGO != nullptr){
        UI::uiGO->RequestDelete();
        UI::uiGO = nullptr;
    }

}

void UI::CreateSkillsGO( AP* ap_behaviour) {

    if(uiGO!=nullptr){
        uiGO->RequestDelete();
        uiGO = nullptr;
    }

    uiGO = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    new Sprite(*uiGO, UI_SCREEN_SPRITE);
    new CameraFollower(*uiGO);

    //resets
    for (const auto skillObj : Skill::skillArrayObj) { //remove skills
        skillObj.lock()->RequestDelete();
    }

    for (int i = Skill::skillArrayObj.size() - 1; i >= 0; i--) { //remove skills
            Skill::skillArrayObj.erase(Skill::skillArrayObj.begin() + i);
    }

    for (unsigned int i = 0; i < Skill::skillArray.size(); i++) {
        int offsetArray = i;
        GameObject* normalSkill = new GameObject(SKILL_SPACE * offsetArray + SKILL_N_OFFSET.x, uiGO->box.y + 123);
        // Acesse o Skill::SkillId a partir do std::shared_ptr<Skill>
        Skill::SkillId skillId = Skill::skillArray[i];
        new Skill(*normalSkill, skillId, ap_behaviour);
        new CameraFollower(*normalSkill);
        auto weak_skill = Game::GetInstance().GetCurrentState().AddObject(normalSkill); 
        Skill::skillArrayObj.push_back(weak_skill);
    }  

    Game::GetInstance().GetCurrentState().AddObject(uiGO);
    
}


void UI::Start() {  
    
    uiGO = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    new Sprite(*uiGO, UI_SCREEN_SPRITE);
    new CameraFollower(*uiGO);
    Game::GetInstance().GetCurrentState().AddObject(uiGO);



    nextArrow = new GameObject();
    new Sprite(*nextArrow, UI_NEXT_SPRITE);

    nextArrow->box.x = RESOLUTION_WIDTH - nextArrow->box.w * nextArrow_TIME_ANIMATION + 87.5;
    nextArrow->box.y = RESOLUTION_HEIGHT * 2/3 - nextArrow->box.h * nextArrow_TIME_ANIMATION;

    Game::GetInstance().GetCurrentState().AddObject(nextArrow);

}



void UI::Update(float dt) { 
    if (nextArrow != nullptr) {

        auto objComponent = nextArrow->GetComponent("Sprite");
        auto objComponentPtr = std::dynamic_pointer_cast<Sprite>(objComponent);
        if (objComponentPtr) {
            auto scaleSprite = objComponentPtr->GetScale();

            // Set the target scale and animation speed
            float targetScale = nextArrow_TIME_ANIMATION;
            float animationSpeed = nextArrow_TIME_ANIMATION_V;


            // Calculate the new scale based on time (dt)
            scaleSprite.x += ScalenextArrow * animationSpeed * dt;

            // Check if the scale has reached the minimum or maximum limit
            if (ScalenextArrow == 1 && scaleSprite.x >= targetScale) {
                // Set the scale to the target value and reverse the direction
                scaleSprite.x = targetScale;
                ScalenextArrow = -1;
            } else if (ScalenextArrow == -1 && scaleSprite.x <= 1.0f) {
                // Set the scale to the target value and reverse the direction
                scaleSprite.x = 1.0f;
                ScalenextArrow = 1;
            }

            //nextArrow->box.x = RESOLUTION_WIDTH - nextArrow->box.w / scaleSprite.x - Camera::pos.x;
            //nextArrow->box.y = RESOLUTION_HEIGHT * 2/3 - nextArrow->box.h / scaleSprite.x - Camera::pos.y;

                
            // Center position original
            auto posXenterX = nextArrow->box.x + nextArrow->box.w / 2;
            auto posXenterY = nextArrow->box.y + nextArrow->box.h / 2;

            // Call SetScale with the correct number of arguments
            if(GameData::playerTurn){
                objComponentPtr->SetScale(scaleSprite.x, scaleSprite.x);
            }
            else{
                objComponentPtr->SetScale(1, 1);
            }

            // Postion correction
            nextArrow->box.x = posXenterX - nextArrow->box.w / 2;
            nextArrow->box.y = posXenterY - nextArrow->box.h / 2;

            /*
            //muito confuso mexer na camera fiz macete
            if(Camera::pos.x != CameraPosBuffer.x){
                nextArrow->box.x -= Camera::pos.x - CameraPosBuffer.x;
                CameraPosBuffer.x = Camera::pos.x;
            }
            if(Camera::pos.y != CameraPosBuffer.y){
                nextArrow->box.y -= Camera::pos.y - CameraPosBuffer.y;
                CameraPosBuffer.y = Camera::pos.y;
                
            }
            */
        }

        
        Vec2 mousePos(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());
        auto& inputManager = InputManager::GetInstance();

        auto nextComponent = nextArrow->GetComponent("Sprite");
        auto nextComponentPtr = std::dynamic_pointer_cast<Sprite>(nextComponent);
        if(nextComponentPtr){
           if (nextArrow->box.Contains(mousePos.x- Camera::pos.x * Game::resizer, mousePos.y- Camera::pos.y * Game::resizer) 
           && GameData::playerTurn == true){
                nextComponentPtr->SetAlpha(255);
                if(inputManager.MousePress(LEFT_MOUSE_BUTTON)){
                    //Skill::selectedSkill = nullptr; 

                    //scenario of skill selection screen
                    if(!Mural::MuralState && !SkillSelection::skillSelectionActivated){
                        UI::nextActivated = true;
                    }            
 
                    else if(Mural::MuralState && !SkillSelection::skillSelectionActivated){
                        Mural::MuralStateActivateReward = true;
                    }

                    else{
                        Skill::skillFromReward = nullptr;
                        Skill::skillToReward = nullptr;
                        SkillSelection::endSkillSelection = true;
                    }
                }
            }else{
                nextComponentPtr->SetAlpha(120); 
            } 
        }
        
 

    }
} 
 
void UI::Render() {

}




bool UI::Is(std::string type){
    return (type == "UI");
}

