#include "Rest.h"
#include "Game.h"
#include "InputManager.h" 
#include "CameraFollower.h"
#include "Camera.h"
#include "GameData.h"
#include "Map.h"
#include "Mother.h"
#include "Daughter.h"




Rest::Rest() 
: State::State(),
daughter(nullptr),
mom(nullptr),
djinn(nullptr),
uiMedicine(nullptr),
uiTea(nullptr),
uiBurn(nullptr),
selectedSFX(nullptr),
medicineActivated(false),
teaActivated(false),
burnActivated(false),
popRest(false)
{

}
   
Rest::~Rest(){
    if(daughter != nullptr){
        daughter->RequestDelete();
        daughter = nullptr;
    }
    delete daughter;

    if(mom != nullptr){
        mom->RequestDelete();
        mom = nullptr;
    }
    delete mom;
    
    if(djinn != nullptr){
        djinn->RequestDelete();
        djinn = nullptr;
    }
    delete djinn;

    if(uiMedicine != nullptr){
        uiMedicine->RequestDelete();
        uiMedicine = nullptr;
    }
    delete uiMedicine;

    if(uiTea != nullptr){
        uiTea->RequestDelete();
        uiTea = nullptr;
    }
    delete uiTea;

    if(uiBurn != nullptr){
        uiBurn->RequestDelete();
        uiBurn = nullptr;
    }
    delete uiBurn;

    if(selectedSFX != nullptr){
        selectedSFX->RequestDelete();
        selectedSFX = nullptr;
    }
    delete selectedSFX;

}

void Rest::Update(float dt){   
    if(popRest){
        popRestTimer.Update(dt);
        if(popRestTimer.Get() >= POP_REST_TIME){
            popRequested = true;
        }
    }

    InputManager& input = InputManager::GetInstance();
    Vec2 mousePos(input.GetMouseX(), input.GetMouseY()); 
 
    // If the event is quit, set the termination flag
    if ((input.KeyPress(ESCAPE_KEY)) || input.QuitRequested()){
        quitRequested = true;
    }

    bool anyActivated = medicineActivated || teaActivated || burnActivated;
    //===============================uiBurn===============================
    if(!anyActivated || burnActivated){
        if(uiBurn->box.Contains(mousePos.x - Camera::pos.x * Game::resizer, mousePos.y- Camera::pos.y * Game::resizer)){
            if(selectedSFX == nullptr){
                selectedSFX = new GameObject();
                Sound *selectSFX_sound = new Sound(*selectedSFX, SKILL_SELECTION); 
                selectSFX_sound->Play(1);

                ChangeSpriteSelected(*uiBurn, REST_UI_BURN_SELECTED);
                ChangeSpriteSelected(*djinn, REST_DJINN_SELECTED);

                burnActivated = true;
            }

            if(input.MousePress(LEFT_MOUSE_BUTTON)){
                UseBurn();
            }
            
        }
        else{ 
                if(selectedSFX != nullptr){
                    selectedSFX->RequestDelete();
                    selectedSFX = nullptr;
                }

                if(burnActivated){
                    ChangeSpriteSelected(*uiBurn, REST_UI_BURN);
                    ChangeSpriteSelected(*djinn, REST_DJINN);
                }
                
                burnActivated = false; 
        }
    }    
    //===============================uiMedicine===============================
    if(!anyActivated || medicineActivated){
        if(uiMedicine->box.Contains(mousePos.x - Camera::pos.x * Game::resizer, mousePos.y- Camera::pos.y * Game::resizer)){
            if(selectedSFX == nullptr){
                selectedSFX = new GameObject();
                Sound *selectSFX_sound = new Sound(*selectedSFX, SKILL_SELECTION); 
                selectSFX_sound->Play(1);

                ChangeSpriteSelected(*uiMedicine, REST_UI_MEDICINE_SELECTED);
                ChangeSpriteSelected(*daughter, REST_DAUGHTER_SELECTED);

                medicineActivated = true;
            }

            if(input.MousePress(LEFT_MOUSE_BUTTON)){
                UseMedicine();
            }
        }
        else{
                if(selectedSFX != nullptr){
                    selectedSFX->RequestDelete();
                    selectedSFX = nullptr;

                }

                if(medicineActivated){
                    ChangeSpriteSelected(*uiMedicine, REST_UI_MEDICINE);
                    ChangeSpriteSelected(*daughter, REST_DAUGHTER);
                }
                

                medicineActivated = false;
            
        }
    }
    //===============================uiTea===============================
    if(!anyActivated || teaActivated){
        if(uiTea->box.Contains(mousePos.x - Camera::pos.x * Game::resizer, mousePos.y- Camera::pos.y * Game::resizer)){
            if(selectedSFX == nullptr){
                selectedSFX = new GameObject();
                Sound *selectSFX_sound = new Sound(*selectedSFX, SKILL_SELECTION); 
                selectSFX_sound->Play(1);

                ChangeSpriteSelected(*uiTea, REST_UI_TEA_SELECTED);
                ChangeSpriteSelected(*mom, REST_MOTHER_SELECTED);

                teaActivated = true;
            }
            
            if(input.MousePress(LEFT_MOUSE_BUTTON)){
                UseTea();
            }
        }
        else{
                if(selectedSFX != nullptr){
                    selectedSFX->RequestDelete();
                    selectedSFX = nullptr;
                }

                if(teaActivated){
                    ChangeSpriteSelected(*uiTea, REST_UI_TEA);
                    ChangeSpriteSelected(*mom, REST_MOTHER);
                }
                

                teaActivated = false;
               
        }    
    }
    

    UpdateArray(dt); 
    Camera::Update(dt);

    State::Update(dt); 
}  

void Rest::ChangeSpriteSelected(GameObject &obj, std::string newSprite){
    // Center position original
    auto posXenterX = obj.box.x + obj.box.w / 2;
    auto posXenterY = obj.box.y + obj.box.h / 2;


    auto component = obj.GetComponent("Sprite");
    auto componentPtr = std::dynamic_pointer_cast<Sprite>(component);
    if(componentPtr){
        obj.RemoveComponent(componentPtr.get());
    }

    new Sprite(obj, newSprite);

    obj.box.x = posXenterX - obj.box.w / 2;
    obj.box.y = posXenterY - obj.box.h / 2;
}

void Rest::LoadAssets(){
    //============================ Background ========================================
    GameObject *bg = new GameObject();
    new Sprite(*bg, REST_BG);
    new CameraFollower(*bg);

    bg->box.x = RESOLUTION_WIDTH  / 2 - bg->box.w / 2;

    AddObject(bg);

    

    //===========================Characters=================================
    //djinn
    djinn = new GameObject();
    new Sprite(*djinn, REST_DJINN);

    djinn->box.x = (RESOLUTION_WIDTH / 2) -  djinn->box.w/2;  
    djinn->box.y = (RESOLUTION_HEIGHT * 2/3 ) +  REST_CHR_OFFSET.y -  djinn->box.h;  
    
    //new CameraFollower(*djinn);
    AddObject(djinn);

    //daughter
    daughter = new GameObject();
    new Sprite(*daughter, REST_DAUGHTER);

    daughter->box.x = (RESOLUTION_WIDTH / 2) -  REST_CHR_OFFSET.x * 3/4 -  daughter->box.w/2;  
    daughter->box.y = (RESOLUTION_HEIGHT * 2/3 ) +  REST_CHR_OFFSET.y -  daughter->box.h;  
    
    //new CameraFollower(*daughter); 
    AddObject(daughter);

    //mom
    mom = new GameObject();
    new Sprite(*mom, REST_MOTHER);

    mom->box.x = (RESOLUTION_WIDTH / 2) +  REST_CHR_OFFSET.x -  mom->box.w/2;  
    mom->box.y = (RESOLUTION_HEIGHT * 2/3 ) +  REST_CHR_OFFSET.y -  mom->box.h;  
     
    //new CameraFollower(*mom);  
    AddObject(mom);


    //============================ UI ========================================  
    GameObject *reader = new GameObject(0, RESOLUTION_HEIGHT * 1/2 + REST_READER_offset);
    new Sprite(*reader, REST_UI_READER); 

    reader->box.x = (RESOLUTION_WIDTH / 2) -  reader->box.w/2;  
  
    new CameraFollower(*reader);
    AddObject(reader);

    //uiMedicine
    uiMedicine = new GameObject(REST_UI_POS.x, RESOLUTION_HEIGHT * 1/2 +  REST_READER_offset  + REST_UI_offset * 3
                                               + REST_UI_POS.y);
    new Sprite(*uiMedicine, REST_UI_MEDICINE); 
    //new CameraFollower(*uiMedicine);
    AddObject(uiMedicine); 

    //uiBurn 
    uiBurn = new GameObject(REST_UI_POS.x, RESOLUTION_HEIGHT * 1/2 + REST_UI_offset + REST_UI_POS.y + REST_READER_offset + REST_UI_offset * 3 + uiMedicine->box.h);
    new Sprite(*uiBurn, REST_UI_BURN); 
    //new CameraFollower(*uiBurn); 
    AddObject(uiBurn);

    //uiTea
    uiTea = new GameObject(REST_UI_POS.x, RESOLUTION_HEIGHT * 1/2 + REST_UI_offset * 2 + REST_READER_offset + REST_UI_offset * 3
                                               + REST_UI_POS.y + uiBurn->box.h + uiMedicine->box.h);
    new Sprite(*uiTea, REST_UI_TEA); 
    //new CameraFollower(*uiTea); 
    AddObject(uiTea);

    Music noncombatMusic;
    noncombatMusic.Open("assets/audio/songNonCombat.mp3");
    noncombatMusic.Play();  


}


void Rest::UseBurn(){
    popRest = true;

    GameData::hpCorrupted -= 15;
    if(GameData::hpCorrupted < 0){
        GameData::hpCorrupted = 0;
    }

    GameObject* selectedSFX = new GameObject();
        Sound *selectSFX_sound = new Sound(*selectedSFX, SKILL_SELECTION_CONFIRMED); 
         selectSFX_sound->Play(1);
}    

void Rest::UseTea(){
    popRest = true;

    GameData::hp += GameData::hpMax * 0.25;
    if(GameData::hp > GameData::hpMax - GameData::hpCorrupted){
        GameData::hp = GameData::hpMax - GameData::hpCorrupted;
    }

    GameObject* selectedSFX = new GameObject();
        Sound *selectSFX_sound = new Sound(*selectedSFX, SKILL_SELECTION_CONFIRMED); 
         selectSFX_sound->Play(1);
}    

void Rest::UseMedicine(){
    popRest = true;

    GameData::life += GameData::lifeMax * 0.25;
    if(GameData::life > GameData::lifeMax){
        GameData::life = GameData::lifeMax;
    }

    GameObject* selectedSFX = new GameObject();
        Sound *selectSFX_sound = new Sound(*selectedSFX, SKILL_SELECTION_CONFIRMED); 
         selectSFX_sound->Play(1);
}    


void Rest::Render(){     
    RenderArray();
    State::Render();
}

void Rest::Start(){
    LoadAssets();
    StartArray();
    started = true;

    Camera::pos.x = 0;
    Camera::pos.y = 0;

}
 
void Rest::Pause(){
    State::Pause();
}

void Rest::Resume(){
    State::Resume();
}

