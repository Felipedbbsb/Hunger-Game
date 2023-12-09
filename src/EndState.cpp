#include "EndState.h"
#include "Game.h"
#include "InputManager.h" 
#include "CameraFollower.h"
#include "Camera.h"


//#include "StartState.h"



EndState::EndState() 
: State::State()
{

}
  
EndState::~EndState(){
    

    
}

void EndState::Update(float dt){   
    InputManager& input = InputManager::GetInstance();
 
 
    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }

    if (input.KeyPress(SPACEBAR_KEY)){
        //StartState* new_stage = new StartState();
        //Game::GetInstance().Push(new_stage); 
        popRequested = true;
    }
        

    UpdateArray(dt); 
    Camera::Update(dt);

    State::Update(dt);
} 

void EndState::LoadAssets(){
    //============================ Background ========================================

    GameObject *bg = new GameObject();
    Sprite* bgSprite= new Sprite(*bg, END_BG);
    CameraFollower *bg_cmfl = new CameraFollower(*bg);
    bg->AddComponent((std::shared_ptr<CameraFollower>)bg_cmfl);       
    bg->AddComponent((std::shared_ptr<Component>)bgSprite);

    bg->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - bg->box.w / 2;

    AddObject(bg);

    //============================ UI ========================================
    //UI takes up 1/3 of the box at the bottom
    GameObject *ui = new GameObject(0, RESOLUTION_HEIGHT * 2/3);
    Sprite* ui_behaviour = new Sprite(*ui, PRESS_SPACE_SPRITE); 
    ui->AddComponent((std::shared_ptr<Sprite>)ui_behaviour); 

    ui->box.x = RESOLUTION_WIDTH * Game::resizer / 2 - ui->box.w / 2;
    ui->box.y = (RESOLUTION_HEIGHT * Game::resizer) * 0.75;


    CameraFollower *ui_cmfl = new CameraFollower(*ui);
    ui->AddComponent((std::shared_ptr<CameraFollower>)ui_cmfl);
    AddObject(ui);
}






void EndState::Render(){     
    RenderArray();
    State::Render();
}

void EndState::Start(){
    LoadAssets();
    StartArray();
    started = true;

}
 
void EndState::Pause(){
    State::Pause();
}

void EndState::Resume(){
    State::Resume();

}

