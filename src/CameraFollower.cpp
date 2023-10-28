#include "CameraFollower.h"

CameraFollower::CameraFollower(GameObject& associated) : Component::Component(associated),
x(associated.box.x),
y(associated.box.y){}

void CameraFollower::Update(float dt){

    associated.box.x = -Camera::pos.x + x;
    associated.box.y = -Camera::pos.y + y;
}

void CameraFollower::Start(float dt){
}

void CameraFollower::Render(){}

bool CameraFollower::Is(std::string type){
    if (type == "CameraFollower"){return true;}
    else{return false;}
}