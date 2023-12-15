#include "CameraShake.h"
#include "Sprite.h"
#include <iostream>

const float ANGLE_VELOCITY = 1.5f;  // Ajuste conforme necessário
const float SCALE_VELOCITY = 0.75;  // Ajuste conforme necessário
const float MAX_ANGLE = 0.05f;        // Ajuste conforme necessário
const float MAX_SCALE_VARIATION = 0.015f;  // Ajuste conforme necessário


CameraShake::CameraShake(GameObject& associated) 
    : Component::Component(associated),
      direction(1),
      x(associated.box.x),
      y(associated.box.y)
{
}

void CameraShake::Update(float dt) {

    auto posXenterX = associated.box.x + associated.box.w / 2;
    auto posXenterY = associated.box.y + associated.box.h / 2;

    shakeTimer.Update(dt);
    float angleVariation = std::sin(shakeTimer.Get() * ANGLE_VELOCITY) * MAX_ANGLE;
    float scaleVariation = std::cos(shakeTimer.Get() * SCALE_VELOCITY) * MAX_SCALE_VARIATION;

    associated.angleDeg += angleVariation;

    auto nextComponent = associated.GetComponent("Sprite");
    if (nextComponent) {
        auto nextComponentPtr = std::dynamic_pointer_cast<Sprite>(nextComponent);
        if (nextComponentPtr) {
            nextComponentPtr->SetScale((SCALE_START + scaleVariation) , (SCALE_START + scaleVariation));
        }
    } 

    associated.box.x = posXenterX - associated.box.w / 2;
    associated.box.y = posXenterY - associated.box.h / 2;

    std::cout << "Angle: " << associated.angleDeg << ", Scale: " << (1.0f + scaleVariation) << std::endl;
}

void CameraShake::Start(float dt){

}

void CameraShake::Render(){}

bool CameraShake::Is(std::string type){
    if (type == "CameraShake"){return true;} 
    else{return false;}
}