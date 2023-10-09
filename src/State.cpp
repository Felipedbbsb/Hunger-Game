#include "State.h"

State::State() : 
    popRequested(false),
    quitRequested(false),
    started(false)
{}

State::~State(){objectArray.clear();}

std::weak_ptr<GameObject> State::AddObject(GameObject* go){
    std::shared_ptr<GameObject> tmp(go);
    objectArray.push_back(tmp);
    if (started){tmp->Start();}
    std::weak_ptr<GameObject> created_go(tmp);
    return created_go;
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject *go){
    for (int i = 0; i < (int)objectArray.size(); i++){
        if (go == objectArray[i].get()){
            std::weak_ptr<GameObject> created_go(objectArray[i]);
            return created_go;
        }
    }
    std::weak_ptr<GameObject> empty_go;
    return empty_go;
}

//====================================================================
//These functions are called by Game when there is a change in
//stack of states. Pause specifies the behavior of a state when a
//another is stacked on top of it. Summary, when the above state is
//unstacked. And Start for when it is created and added to the stack.
//====================================================================

void State::Start(){
    LoadAssets();
    for (int i = 0; i < (int)objectArray.size(); i++){
        objectArray[i]->Start();
    }
    started = true;
}

void State::Pause(){
}

void State::Resume(){
}


//==================================================================
//These functions are called by Game every frame. They are pure
//as you may want to do some computation/rendering before or after
//of objects and, in this way, this becomes possible.
//==================================================================

void State::Update(float dt){
}

void State::Render(){
}

//==================================================================
//These functions are used by Game
//to control the change of states or the end of the game.
//==================================================================

bool State::PopRequested(){return popRequested;}

bool State::QuitRequested(){return quitRequested;}

//==================================================================



void State::StartArray(){
    for (int i = 0; i < (int)objectArray.size(); i++){
        objectArray[i]->Start();
    }
}

void State::UpdateArray(float dt){
    for (int i = 0; i < (int)objectArray.size(); i++){
        objectArray[i]->Update(dt);
    }

    for (int i = (int)objectArray.size() - 1; i >= 0; --i){
        if (objectArray[i]->IsDead()){
            objectArray.erase(objectArray.begin() + i);
        }
    }
}

void State::RenderArray(){
    for (int i = 0; i < (int)objectArray.size(); i++){
        objectArray[i]->Render();
    }
} 