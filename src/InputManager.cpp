#include "InputManager.h"


//Meyers’ Singleton.
InputManager &InputManager::GetInstance(){
    static InputManager ins;
    return ins;
}

InputManager::InputManager() : 
mouseState{false, false, false, false, false, false},
mouseUpdate{0, 0, 0, 0, 0, 0},
quitRequested(false),
updateCounter(0),
mouseX(0),
mouseY(0){}

InputManager::~InputManager(){}

void InputManager::Update() {
    SDL_Event event;
    SDL_GetMouseState(&mouseX, &mouseY);
    updateCounter++;
    quitRequested = false;

    while (SDL_PollEvent(&event)) {
        if (!event.key.repeat) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    keyState[event.key.keysym.sym] = true;
                    keyUpdate[event.key.keysym.sym] = updateCounter;
                    break;
                case SDL_KEYUP:
                    keyState[event.key.keysym.sym] = false;
                    keyUpdate[event.key.keysym.sym] = updateCounter;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouseState[event.button.button] = true;
                    mouseUpdate[event.button.button] = updateCounter;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouseState[event.button.button] = false;
                    mouseUpdate[event.button.button] = updateCounter;
                    break;
                case SDL_QUIT:
                    quitRequested = true;
                    break;
            }
        }
    }
}

bool InputManager::QuitRequested() { return quitRequested; }

int InputManager::GetMouseX() { return mouseX; }

int InputManager::GetMouseY() { return mouseY; }


//For next functions

// ___Press and ___Release are interested in the pressure that occurred
// in that frame, and should only return true in that case. Use vectors
// ___Update and the updateCounter to know. Is___Down returns if the button/key
// is pressed, regardless of when it occurred.


bool InputManager::KeyPress(int key) {
    return keyState[key] && keyUpdate[key] == updateCounter;
}

bool InputManager::KeyRelease(int key) {
    return !keyState[key] && keyUpdate[key] == updateCounter;
}

bool InputManager::IsKeyDown(int key) {
    return keyState[key];
}

bool InputManager::MousePress(int button) {
    return mouseState[button] && mouseUpdate[button] == updateCounter;
}

bool InputManager::MouseRelease(int button) {
    return !mouseState[button] && mouseUpdate[button] == updateCounter;
}

bool InputManager::IsMouseDown(int button) {
    return mouseState[button];
}

