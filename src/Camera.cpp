#include "Camera.h"
#include "InputManager.h"
#include "Game.h"
#include "CombatState.h"

#define CAMERA_SPEED 500 //movement speed
#include <iostream>
#include <cstdlib> 


GameObject* Camera::focus;

Vec2 Camera::pos;
Vec2 Camera::speed;


void Camera::Update(float dt) {
    if (focus == nullptr) {
        InputManager& input = InputManager::GetInstance();
        //speed.x = static_cast<float>((input.IsKeyDown(RIGHT_ARROW_KEY) - input.IsKeyDown(LEFT_ARROW_KEY)) * CAMERA_SPEED);
        //speed.y = static_cast<float>((input.IsKeyDown(DOWN_ARROW_KEY) - input.IsKeyDown(UP_ARROW_KEY)) * CAMERA_SPEED);
        pos.x += dt * speed.x;
        pos.y += dt * speed.y;

    }
    else {
        

        // Calculate the difference between camera position and focus position
        Vec2 diff;
        diff.x = focus->box.x  - pos.x;
        diff.y = focus->box.y - pos.y;


        // Move the camera towards the focus position
        if(std::abs(diff.x) < 2){
            pos.x  = focus->box.x;
        }
        else{
            pos.x += dt * diff.x * 2;
        }
        
        // Move the camera towards the focus position
        if(std::abs(diff.y) < 2){
            pos.y  = focus->box.y;
            
        }
        else{
            pos.y += dt * diff.y * 2;
        }

        if(diff.x == 0 && diff.y == 0){
            CombatState::ChangingSides = false;
            focus->RequestDelete();
            Camera::Unfollow();
        }

    }

}

void Camera::Follow(GameObject* newFocus){
    focus = newFocus;
}

void Camera::Unfollow(){
    focus = nullptr;
}

