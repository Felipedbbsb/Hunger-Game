#pragma once

#include "GameObject.h"
#include <memory>
#include <SDL2/SDL.h>


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

        virtual void FadeScreen(bool fadingOut, float fadeFactor);
        virtual void SetFadeTime(float fadeTime = 0.5); //Default

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

        bool fadingOut;  // Flag to indicate whether the screen is fading out
        float fadeFactor; // Factor to control the alpha value for fading
        float fadeTime; // Factor to control the alpha value for fading

        std::vector<std::shared_ptr<GameObject>> objectArray;

};