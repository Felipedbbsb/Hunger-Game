#include "InteractionState.h"
#include "Game.h"
#include "InputManager.h"
#include "CameraFollower.h"
#include "Camera.h"
#include "Text.h"
#include "UI.h"
#include "Mother.h"
#include "Daughter.h"
#include "Enemies.h" 
#include "Skill.h"
#include "CombatState.h"
#include "GameData.h"
#include "Papiro.h"
#include "Resources.h"

InteractionState::InteractionState(std::vector<Enemies::EnemyId> enemiesArrayIS, Skill::AttackType attackType, Skill::TargetType whoAttacks, Skill::TargetType whoReceives)
    : State(),
      interactionTime(){

    Game::GetInstance().TakeScreenshot("screenshot.png"); // Especifique o nome do arquivo para o screenshot

    // Carregar a textura do screenshot se estiver disponível
    auto screenshotTexture = "screenshot.png"; // Use o nome do arquivo que você especificou

    if (screenshotTexture) {
        // Crie um GameObject com um Sprite do screenshot
        GameObject* screenshotObject = new GameObject();
        Sprite* screenshotSprite = new Sprite(*screenshotObject, screenshotTexture);
        screenshotObject->AddComponent(std::shared_ptr<Component>(screenshotSprite));
        AddObject(screenshotObject);
    }

    //============================ Background ========================================
    GameObject *papiro = new GameObject();
    Papiro *papiro_behaviour = new Papiro(*papiro, enemiesArrayIS, attackType, whoAttacks, whoReceives);
    papiro->AddComponent(std::shared_ptr<Component>(papiro_behaviour));
    AddObject(papiro);
} 

InteractionState::~InteractionState() {

}


void InteractionState::Update(float dt) {
    InputManager &input = InputManager::GetInstance();

    Camera::Update(dt);

    // If the event is quit, set the termination flag
    if (input.KeyPress(ESCAPE_KEY) || input.QuitRequested()) {
        quitRequested = true;
    }

    interactionTime.Update(dt);
    if (interactionTime.Get() >= INTERACTION_COOLDOWN) {
        interactionTime.Restart();
        popRequested = true;
    }


    UpdateArray(dt);
}

void InteractionState::LoadAssets() {
    // Carregue os recursos necessários para o estado
}

void InteractionState::Render() {
    RenderArray();
}

void InteractionState::Start() {
    LoadAssets();
    StartArray();
    started = true;

    CombatState::InteractionSCreenActivate = false;

}

void InteractionState::Pause() {

}

void InteractionState::Resume() {

}
