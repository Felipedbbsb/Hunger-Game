#include "CameraParallax.h"
#include "Game.h"
CameraParallax::CameraParallax(GameObject& associated, float parallaxFactor) : Component::Component(associated),
x(associated.box.x),
y(associated.box.y),
parallaxFactor(parallaxFactor){}

void CameraParallax::Update(float dt){

    associated.box.x = (-Camera::pos.x * parallaxFactor + x) * Game::resizer;
    associated.box.y = (-Camera::pos.y * parallaxFactor + y) * Game::resizer;
}

void CameraParallax::Start(float dt){
}

void CameraParallax::Render(){}

bool CameraParallax::Is(std::string type){
    if (type == "CameraParallax"){return true;}
    else{return false;}
}