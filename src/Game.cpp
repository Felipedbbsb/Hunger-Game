#include <string>
#include <iostream>
#include "Resources.h"
#include "Game.h"
#include "InputManager.h"

 
#define AUDIO_CHUNKSIZE 1024
#define AUDIO_FREQUENCY MIX_DEFAULT_FREQUENCY
#define AUDIO_FORMAT MIX_DEFAULT_FORMAT
#define AUDIO_CHANNELS MIX_DEFAULT_CHANNELS 
#define SOUND_RESOLUTION 64



// Screen definition
 
#define SCREEN_TITLE "Hunger"

#define WINDOW_FLAGS 0 // Ex.: SDL_WINDOW_FULLSCREEN

#include <stdexcept> // Para usar exceções

// Static class member initialization
Game *Game::instance = nullptr;

float Game::resizer  = 1;

Game::Game(std::string title, int width, int height) : 
    frameStart(0), 
    dt(0.0),
    storedState(nullptr){

    if (Game::instance != nullptr) {
        throw std::runtime_error("Something's Wrong!");
    } else {
        Game::instance = this;
    }

    try {
        if (!InitializeSDL() || !InitializeIMG() || !InitializeMixer() || !InitializeTTF() ||!CreateWindowAndRenderer(title, width, height, WINDOW_FLAGS)) {
            // Tratar falhas de inicialização aqui, se necessário.
            throw std::runtime_error("Initialization failed");
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Initialization error: " << e.what() << std::endl;
        // Faça o tratamento de erro adequado aqui, como sair do jogo.
        // Você pode até mesmo lançar uma exceção aqui para indicar que a inicialização do jogo falhou.
    }

}

bool Game::InitializeSDL() {
    int SDL_FAIL = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    if (SDL_FAIL != 0) {
        throw std::runtime_error("SDL_Init Failed");
    }
    return true;
}

bool Game::InitializeIMG() {
    int IMG_FAIL = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
    if (IMG_FAIL == 0) {
        throw std::runtime_error("IMG_Init Failed");
    }
    return true;
}

bool Game::InitializeMixer() {
    int MSC_FAIL = Mix_Init(MIX_INIT_FLAC | MIX_INIT_OGG | MIX_INIT_MP3);
    if (MSC_FAIL == 0) {
        throw std::runtime_error("No loader was loaded");
    }

    Mix_OpenAudio(AUDIO_FREQUENCY, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNKSIZE);
    Mix_AllocateChannels(SOUND_RESOLUTION);

    return true;
}

bool Game::InitializeTTF() {
    int TTF_FAIL = TTF_Init();;
    
    if (TTF_FAIL == 1) {
        throw std::runtime_error("TTF_Init Failed");
    }

    return true;
}

bool Game::CreateWindowAndRenderer(const std::string& title, int width, int height, int flags) {
    
    int maxScreenWidth, maxScreenHeight;
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
        throw std::runtime_error("Failed to get display mode");
    } else {
        maxScreenWidth = dm.w;
        maxScreenHeight = dm.h; 
    }
    
    float widthRatio = static_cast<float>(maxScreenWidth) / RESOLUTION_WIDTH;
    float heightRatio = static_cast<float>(maxScreenHeight) / RESOLUTION_HEIGHT;
    resizer = std::min(widthRatio, heightRatio);
  
    // Calcule a largura e altura da janela de acordo com a resolução máxima
    int windowWidth = static_cast<int>(RESOLUTION_WIDTH * resizer); 
    int windowHeight = static_cast<int>(RESOLUTION_HEIGHT * resizer);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, flags);
    if (window == nullptr) {
        throw std::runtime_error("Window creation failed");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE); 
    //SDL_SetHint(SDL_HINT_RENDERER_SCALE_QUALITY, “linear”);
    
    if (renderer == nullptr) { 
        throw std::runtime_error("Failed to create renderer");
    }   
 
 
    //Render logical size, rezise screen based on resolution screen
     if(SDL_RenderSetLogicalSize(renderer, RESOLUTION_WIDTH, RESOLUTION_HEIGHT)){
        throw std::runtime_error("Failed to set logical size");
    }

    return true;
}
 


//A ordem importa! Faça na ordem inversa da inizialização
Game::~Game() {
    // Clean up the state stack using a while loop 
    while (!stateStack.empty()) {
        // Delete and pop each state, ensuring proper resource release
        stateStack.pop();
    }

    // Release the stored state if it is not nullptr 
    if (storedState != nullptr) {
        delete storedState;
    }
 
    Resources::ClearImages();
	Resources::ClearSounds();
	Resources::ClearMusics();
    Resources::ClearFonts();

    // Clean up SDL and SDL_image resources
    Mix_CloseAudio();               // Close audio subsystem
    Mix_Quit();                     // Quit SDL_mixer
    IMG_Quit();                     // Quit SDL_image
    TTF_Quit();
    SDL_DestroyRenderer(renderer);  // Destroy SDL renderer
    SDL_DestroyWindow(window);      // Destroy SDL window
    SDL_Quit();                     // Quit SDL
}


//It is used to prevent a class from having more than one instance.
Game& Game::GetInstance() {
    std::string title = SCREEN_TITLE;
    // dimensoes da janela do jogo
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;

    if (Game::instance != nullptr) {
        return *Game::instance;
    } else {
        Game::instance = new Game(title, width, height);
        return *Game::instance;
    }
}

State &Game::GetCurrentState(){
    return *stateStack.top();
}

SDL_Renderer* Game::GetRenderer() {
    return renderer;
}

void Game::Push(State *state){ 
    storedState = state;
}
 

//GAME LOOP

void Game::Run() {
    if (storedState != nullptr){
        stateStack.push((std::unique_ptr<State>)storedState);
        stateStack.top()->Start();
        storedState = nullptr;
    }
    else {
	    return;
	}

    while (!stateStack.empty() && !stateStack.top()->QuitRequested() ) {
        // Check if the top state wants to pop
        if (stateStack.top()->PopRequested()) {
            stateStack.top()->Pause();
            std::cout << "Antes de stateStack.pop(). Tamanho da pilha: " << stateStack.size() << std::endl;
            stateStack.pop();
            std::cout << "Depois de stateStack.pop(). Tamanho da pilha: " << stateStack.size() << std::endl;
            Resources::ClearImages();
			Resources::ClearSounds();
			Resources::ClearMusics();
            Resources::ClearFonts();

            if (!stateStack.empty()) {
                stateStack.top()->Resume();
            }

            
        }
        // Check if there's a stored state to push
        if (storedState != nullptr) { 
            if (!stateStack.empty()) {
                stateStack.top()->Pause();
            } 
            stateStack.emplace((std::unique_ptr<State>)storedState); // Use std::move to transfer ownership
            stateStack.top()->Start();
            storedState = nullptr;
        }  
        else if (stateStack.empty()) {
			break;
		}
        
        CalculateDeltaTime();
        InputManager::GetInstance().Update();
        auto& currentTopState = stateStack.top();
        currentTopState->Update(dt);
        currentTopState->Render(); 
        SDL_RenderPresent(Game::GetInstance().GetRenderer());
    }   

   
    Resources::ClearImages(); 
    Resources::ClearMusics();
    Resources::ClearSounds();
    Resources::ClearFonts();
}



void Game::CalculateDeltaTime(){
    int time_delta  = SDL_GetTicks();


    dt = (time_delta - frameStart) / 1000.0; // converting time from miliseconds to seconds
    //std::cout << "Frames = " << 1000/dt <<std::endl; 
    frameStart = time_delta;
}

float Game::GetDeltaTime(){return dt;} 



void Game::TakeScreenshot(std::string filename) {
    SDL_Surface* screenshotSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    if (screenshotSurface) {
        SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, screenshotSurface->pixels, screenshotSurface->pitch);
        IMG_SavePNG(screenshotSurface, filename.c_str());
        SDL_FreeSurface(screenshotSurface);
    }
}






