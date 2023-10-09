#pragma once


#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE

#include "SDL_include.h"
#include "Component.h"
#include "Vec2.h"
#include "Timer.h"
#include "GameObject.h"

//Class responsible for load and render of images
class Sprite : public Component{
  public:
    Sprite(GameObject &associated);
    Sprite(GameObject &associated, std::string file, 
            int frameCount = 1, 
            float frameTime = 1,
            float secondsToSelfDestruct = 0);

    ~Sprite();
    void Open(std::string file);
    void SetClip(int x, int y,int w, int h);
    void Render();
    void Render(int x, int y);
    int GetWidth();
    int GetHeight();

    void SetScale(float scaleX, float scaleY);
    Vec2 GetScale();
    bool IsOpen();
    void Update(float dt);
    bool Is(std::string type);
    
    void SetFrame(int frame);
    void SetFrameCount(int frameCount);
    void SetFrameTime(float frameTime);
    void SetSelfDestruct(float frameTime);

  private:
    SDL_Texture *texture;
    int width;
    int height;
    Vec2 scale;
    SDL_Rect clipRect;

    int frameCount; 
    int currentFrame; 
    float timeElapsed;
    float frameTime;

    Timer selfDestructCount;
    float secondsToSelfDestruct;
};