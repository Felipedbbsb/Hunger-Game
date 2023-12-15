#include "SandParticles.h"
#include "Camera.h"

SandParticles::SandParticles(SDL_Renderer* renderer, int screenWidth, int screenHeight, float maxParticleLife, int numParticles, int minParticleSize, int maxParticleSize)
    : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight), particlesActive(false), maxParticleLife(maxParticleLife), numParticles(numParticles), minParticleSize(minParticleSize), maxParticleSize(maxParticleSize) {
    // Inicializa as partículas
    initializeParticles();
}

SandParticles::~SandParticles() {
    // Limpa as partículas
    particles.clear();
}

void SandParticles::toggleParticles() {
    particlesActive = !particlesActive;
}

void SandParticles::Update(float dt) {
    if (particlesActive) {
        for (auto& particle : particles) {
            // Atualiza a posição da partícula
            particle.x -= particle.speed;

            // Atualiza o tempo de vida da partícula
            particle.life -= dt;

            // Calcula o fator alpha com base na porcentagem de tempo restante
            float alphaPercentage = particle.life / maxParticleLife;
            particle.alpha = static_cast<Uint8>(alphaPercentage * 150);

            // Reinicia a partícula quando ela atinge a borda ou o tempo de vida é zero
            if (particle.x < 0 || particle.life <= 0) {
                particle.x = rand() % screenWidth;
                particle.y = rand() % screenHeight;

                // Ajusta a faixa de velocidade conforme necessário 
                int velocityRange = static_cast<int>((MAX_VELOCITY - MIN_VELOCITY) * 1000.0);
                particle.speed = (rand() % velocityRange + static_cast<int>(MIN_VELOCITY * 1000.0)) / 1000.0;

                // Ajusta a faixa de tamanhos conforme necessário
                particle.size = rand() % (maxParticleSize - minParticleSize) + minParticleSize;
                particle.life = maxParticleLife; // Reinicia o tempo de vida
                particle.alpha = 150;  // Reinicia o canal alpha
            }
        } 
    } 
}

void SandParticles::Render() {
    if (particlesActive) {
        for (const auto& particle : particles) {
            int particleX = static_cast<int>(particle.x + Camera::pos.x);
            int particleY = static_cast<int>(particle.y);

            // Modificado para usar SDL_Rect
            SDL_Rect particleRect = { particleX, particleY, particle.size, particle.size };
            SDL_SetRenderDrawColor(renderer, particleColor.r, particleColor.g, particleColor.b, particle.alpha);
            SDL_RenderFillRect(renderer, &particleRect);
        }
    }
}

void SandParticles::initializeParticles() {
    particles.clear();
    // Ajusta os parâmetros conforme necessário para a simulação
    for (int i = 0; i < numParticles; ++i) {
        Particle particle;
        particle.x = rand() % screenWidth;
        particle.y = rand() % screenHeight;
        particle.speed = rand() % 3 + 1; // Ajusta a faixa de velocidade conforme necessário
        particle.size = rand() % (maxParticleSize - minParticleSize) + minParticleSize; // Ajusta a faixa de tamanhos conforme necessário
        particle.life = static_cast<float>(rand() % static_cast<int>(maxParticleLife * 1000)) / 1000.0f; // Tempo de vida inicial aleatório
        particle.alpha = 150;  // Inicializa o canal alpha
        particles.push_back(particle); 
    }
}