#pragma once



#include <iostream>
#include <memory>
#include <algorithm> 


#include "State.h" 
#include "GameObject.h"
#include "Sprite.h"
#include "Component.h"
#include "Enemies.h"
#include "Music.h"
#include "Timer.h"

#define REST_BG "assets/img/UI/rest/background.png"

#define REST_CHR_OFFSET Vec2(300, 300)

#define REST_DAUGHTER "assets/img/UI/rest/Sprites/flowerDaughter.png"
#define REST_DAUGHTER_SELECTED "assets/img/UI/rest/Sprites/flowerDaughterSelected.png"

#define REST_DJINN "assets/img/UI/rest/Sprites/flowerDjinn.png"
#define REST_DJINN_SELECTED "assets/img/UI/rest/Sprites/flowerDjinnSelected.png"

#define REST_MOTHER "assets/img/UI/rest/Sprites/flowerMom.png"
#define REST_MOTHER_SELECTED "assets/img/UI/rest/Sprites/flowerMomSelected.png"

#define REST_UI_POS Vec2(75, 250)

#define REST_UI_offset 25

#define REST_READER_offset 30

#define REST_UI_BURN "assets/img/UI/rest/UI/burn.png"
#define REST_UI_BURN_SELECTED "assets/img/UI/rest/UI/burnSelected.png"

#define REST_UI_MEDICINE "assets/img/UI/rest/UI/medicine.png"
#define REST_UI_MEDICINE_SELECTED "assets/img/UI/rest/UI/medicineSelected.png"

#define REST_UI_TEA "assets/img/UI/rest/UI/tea.png"
#define REST_UI_TEA_SELECTED "assets/img/UI/rest/UI/teaSelected.png"

#define REST_UI_READER "assets/img/UI/rest/UI/reader.png"

#define POP_REST_TIME 0.25

class Rest : public State{
    public:
        Rest();
        ~Rest();
        void LoadAssets();
        void Update(float dt);
        void Render();
        void Start();
        void Pause();
        void Resume();

        void ChangeSpriteSelected(GameObject &obj, std::string newSprite);
        
    private:

        void UseBurn();
        void UseTea();
        void UseMedicine();

        GameObject* daughter;
        GameObject* mom; 
        GameObject* djinn;  
        GameObject* uiMedicine; 
        GameObject* uiTea; 
        GameObject* uiBurn; 
        GameObject* selectedSFX;

        bool medicineActivated;
        bool teaActivated;
        bool burnActivated;

        bool popRest;
        Timer popRestTimer;
};