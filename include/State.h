#pragma once

#include "GameObject.h"
#include <memory>

class State{
    public:
        State();
        virtual ~State();

        virtual void LoadAssets() = 0;
        virtual void Update(float dt);
        virtual void Render();

        virtual void Start();
        virtual void Pause();
        virtual void Resume();

        virtual std::weak_ptr<GameObject> AddObject(GameObject* go);
        virtual std::weak_ptr<GameObject> GetObjectPtr(GameObject* go);

        bool PopRequested();
        bool QuitRequested();

    
    protected:
        void virtual StartArray();
        void virtual UpdateArray(float dt);
        void virtual RenderArray();
        bool popRequested;
        bool quitRequested;
        bool started;

        std::vector<std::shared_ptr<GameObject>> objectArray;

};