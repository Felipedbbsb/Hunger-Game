#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "Rect.h"
#include "Component.h"

class Component;

class GameObject{
  public:
    GameObject();
    GameObject(int posX, int posY);
    GameObject(Vec2 pos);
    ~GameObject();
    void Start();
    void Update(float dt);
    void Render();
    bool IsDead();
    void RequestDelete();
    /// ----------------
    void AddComponent(std::shared_ptr<Component> cpt);
    void RemoveComponent(std::shared_ptr<Component> cpt);
    std::shared_ptr<Component> GetComponent(std::string type);
    double angleDeg;
    Rect box;
    void NotifyCollision(GameObject& other);


    double GetAngleRad();

  private:
    std::vector<std::shared_ptr<Component>> components;
    bool isDead;
    bool started;
    
};