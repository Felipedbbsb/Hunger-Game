#include "Collider.h"



#ifdef DEBUG
#include "Camera.h"
#include "Game.h"

#include <SDL2/SDL.h>
#endif //DEBUG

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset) : Component::Component(associated),
scale(scale),
offset(offset)
{}

void Collider::Update(float dt){
    // Copy the dimensions of the associated object's bounding box
    this->box = associated.box;

    // Scale the bounding box dimensions
    this->box.w *= scale.x;
    this->box.h *= scale.y;

    // Calculate the rotation in degrees
    float rotationInDegrees = offset.RotateDegree();

    // Update the center of the bounding box based on the scaled offset and rotation
    this->box.DefineCenter(associated.box.GetCenter() + Vec2(rotationInDegrees, 0) + offset);

} 
 
void Collider::Render(){
#ifdef DEBUG
    Vec2 center(box.GetCenter());
    SDL_Point points[5];

    Vec2 point = (Vec2(box.x, box.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = (Vec2(box.x + box.w, box.y) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[1] = {(int)point.x, (int)point.y};

    point = (Vec2(box.x + box.w, box.y + box.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[2] = {(int)point.x, (int)point.y};

    point = (Vec2(box.x, box.y + box.h) - center).RotateVector(associated.angleDeg / (180 / 3.14159265359)) + center + Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

bool Collider::Is(std::string type){
    return (type == "Collider");
}

void Collider::SetScale(Vec2 scale){
    this->scale = scale;
}

void Collider::SetOffset(Vec2 offset){
    this->offset = offset;
}

void Collider::NotifyCollision(GameObject &other)
{}