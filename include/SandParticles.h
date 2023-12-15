
#pragma once

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE

#define APLHA_FADEOUT_VELOCITY 2.0f

#define APLHA_FADEOUT_VELOCITY 2.0f

#define MAX_VELOCITY 6.0
#define MIN_VELOCITY 1.0

#include "SDL_include.h"
#include <vector>

#include "Camera.h"


class SandParticles {
public:
    SandParticles(SDL_Renderer* renderer, int screenWidth, int screenHeight, float maxParticleLife, int numParticles, int minParticleSize, int maxParticleSize);

    ~SandParticles();

    void toggleParticles();
    void Update(float dt);
    void Render();

private:
    struct Particle {
        int x;
        int y;
        int speed;
        int size;
        float life;
        Uint8 alpha;  // Adiciona o canal alpha
    };

    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;
    std::vector<Particle> particles;
    SDL_Color particleColor = {205, 180, 70, 255}; // Branco por padrão
    bool particlesActive;

    float maxParticleLife;
    int numParticles;
    int minParticleSize;
    int maxParticleSize;

    // Inicializa as partículas
    void initializeParticles();
};


