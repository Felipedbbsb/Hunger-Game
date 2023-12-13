#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject& associated) : associated(associated) {
    associated.AddComponent(this);
}
Component::~Component(){
}

void Component::Start()
{
}

void Component::NotifyCollision(GameObject& other)
{   
}