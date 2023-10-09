#include "Sprite.h"
#include "Game.h"
#include "GameObject.h"
#include "Resources.h"
#include "Camera.h"

#define SETCLIP_X 0
#define SETCLIP_Y 0

Sprite::Sprite(GameObject &associated) : Component::Component(associated),
scale(Vec2(1, 1)),
currentFrame(0),
timeElapsed(0){
    texture = nullptr;
}

Sprite::Sprite(GameObject &associated, std::string file, int frameCount, float frameTime, float secondsToSelfDestruct) : Sprite(associated){
    // Define o tempo entre frames e o número total de frames
    this->frameTime = frameTime;
    this->frameCount = frameCount;
    //SetFrameCount(frameCount);
    this->secondsToSelfDestruct = secondsToSelfDestruct;
    Open(file);
}

Sprite::~Sprite(){}

void Sprite::Open(std::string file) {
    // Carrega a imagem indicada pelo caminho 'file'
    texture = Resources::GetImage(file.c_str());

    if (texture == nullptr){
        std::cerr << "Failed to load texture: " << file << std::endl; // falha em carregar imagem.
    } 
    else {
        std::cout << "Texture loaded successfully: " << file << std::endl;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    }
    SetClip(SETCLIP_X, SETCLIP_Y, width, height);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    // Define o retângulo de clipagem
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;

    // Define o tamanho da hitbox
    associated.box.w = w * scale.x;
    associated.box.h = h * scale.y;
}

void Sprite::Render() {
    int RENDER_FAIL;
    SDL_Rect dstLoc = {int(associated.box.x) + (int)Camera::pos.x, int(associated.box.y) + (int)Camera::pos.y, (int)(clipRect.w * GetScale().x), (int)(clipRect.h * GetScale().y)};

    // Renderiza a textura com a clipagem adequada
    RENDER_FAIL = SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstLoc, associated.angleDeg, nullptr, SDL_FLIP_NONE);
    if (RENDER_FAIL != 0) {
        std::cerr << "Texture render failure: " << SDL_GetError() << std::endl; // falha ao renderizar textura
    }
}




//Render é um wrapper para SDL_RenderCopy, que recebe quatro argumentos.
// -SDL_Renderer* renderer: O renderizador de Game.
// -SDL_Texture* texture: A textura a ser renderizada;
// -SDL_Rect* srcrect: O retângulo de clipagem. Especifica uma área da textura a ser "recortada" e renderizada.
//- SDL_Rect* dstrect: O retângulo destino. Determina a posição na tela
//em que a textura deve ser renderizada (membros x e y). Se os membros
//w e h diferirem das dimensões do clip, causarão uma mudança na
//escala, contraindo ou expandindo a imagem para se adaptar a esses
//valores.

void Sprite::Render(int x, int y){
    int RENDER_FAIL;
    SDL_Rect dstLoc = {(int)(x * GetScale().x) + (int)Camera::pos.x, (int)(y * GetScale().y) + (int)Camera::pos.y, (int)(clipRect.w * GetScale().x), (int)(clipRect.h * GetScale().y)};
    
    // Renderiza a textura em uma posição específica
    RENDER_FAIL = SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstLoc, associated.angleDeg, nullptr, SDL_FLIP_NONE);
    if (RENDER_FAIL != 0){
        std::cerr << "Texture render failure: " << SDL_GetError() << std::endl; // falha ao renderizar textura
    }
}

int Sprite::GetWidth(){   
    return (width * scale.x) / frameCount;
}

int Sprite::GetHeight() {
    return height * scale.y;
}

Vec2 Sprite::GetScale(){return scale;}

void Sprite::SetScale(float X_scale, float Y_scale){
    if (X_scale != 0){
       scale.x = X_scale;
       associated.box.w = clipRect.w * scale.x;
    }
    if (Y_scale != 0){
       scale.y = Y_scale;
       associated.box.h = clipRect.h * scale.y;
    }
}

bool Sprite::IsOpen() {
    return (texture != nullptr);
}

void Sprite::Update(float dt) {

    if (secondsToSelfDestruct > 0)
    {   
        selfDestructCount.Update(dt);
        if (selfDestructCount.Get() >= secondsToSelfDestruct)
        {
            associated.RequestDelete();
        }
    }


    timeElapsed += dt; // Verifica em que momento de frame o sprite está
    if (timeElapsed >= frameTime) { // Passa para o próximo frame
        currentFrame = (currentFrame + 1) % frameCount; // Evita que currentFrame ultrapasse frameCount
        
        SetFrame(currentFrame);
        timeElapsed = 0;
    }
}

void Sprite::SetFrame(int frame){   
    timeElapsed = 0;
    currentFrame = frame;
    SetClip(currentFrame * (GetWidth() / scale.x), SETCLIP_Y, GetWidth() / scale.x, GetHeight() / scale.y );
}

void Sprite::SetFrameCount(int frameCount){
    this->frameCount = frameCount;
    SetFrame(0);
    associated.box.w = GetWidth();
    associated.box.DefineCenter(associated.box.x, associated.box.y);
}

void Sprite::SetFrameTime(float frameTime){   
    this->frameTime = frameTime;
}

void Sprite::SetSelfDestruct(float frameTime){   
    this->secondsToSelfDestruct = secondsToSelfDestruct;
}

bool Sprite::Is(std::string type){
    return (type == "Sprite");
}
