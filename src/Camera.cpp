#include "Camera.h"
#include "InputManager.h"
#include "Game.h"

#define CAMERA_SPEED 500 //movement speed

GameObject* Camera::focus;

Vec2 Camera::pos;
Vec2 Camera::speed;


void Camera::Update(float dt) {
    if (focus == nullptr) {
        InputManager& input = InputManager::GetInstance();
        speed.x = static_cast<float>((input.IsKeyDown(RIGHT_ARROW_KEY) - input.IsKeyDown(LEFT_ARROW_KEY)) * CAMERA_SPEED);
        speed.y = static_cast<float>((input.IsKeyDown(DOWN_ARROW_KEY) - input.IsKeyDown(UP_ARROW_KEY)) * CAMERA_SPEED);
    }
    else {
        pos.x = -(focus->box.GetCenter().x) + (SCREEN_WIDTH / 2);
        pos.y = -(focus->box.GetCenter().y) + (SCREEN_HEIGHT / 2);
    }

    pos.x += dt * speed.x;
    pos.y += dt * speed.y;
}

void Camera::Follow(GameObject* newFocus){
    focus = newFocus;
}

void Camera::Unfollow(){
    focus = nullptr;
}

