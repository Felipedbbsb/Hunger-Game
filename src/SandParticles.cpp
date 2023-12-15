#include "SandParticles.h"
#include "Camera.h"

SandParticles::SandParticles(SDL_Renderer* renderer, int screenWidth, int screenHeight, float maxParticleLife, int numParticles, int minParticleSize, int maxParticleSize)
    : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight), particlesActive(false), maxParticleLife(maxParticleLife), numParticles(numParticles), minParticleSize(minParticleSize), maxParticleSize(maxParticleSize) {
    // Initialize the particles
    initializeParticles();
}

SandParticles::~SandParticles() {
    // Cleans particles
    particles.clear();
}

void SandParticles::toggleParticles() {
    particlesActive = !particlesActive;
}

void SandParticles::Update(float dt) {
    if (particlesActive) {
        for (auto& particle : particles) {
            
            // Update particle position
            particle.x -= particle.speed;

            // Update particle lifetime
            particle.life -= dt;

            // Calculates the alpha factor based on the percentage of time remaining
            float alphaPercentage = particle.life / maxParticleLife;
            particle.alpha = static_cast<Uint8>(alphaPercentage * 150);
 
            // Restarts the particle when it reaches the edge or the lifetime is zero
            if (particle.x < 0 || particle.life <= 0) {
                particle.x = rand() % screenWidth;
                particle.y = rand() % screenHeight;

                // Adjust speed range as needed
                int velocityRange = static_cast<int>((MAX_VELOCITY - MIN_VELOCITY) * 1000.0);
                particle.speed = (rand() % velocityRange + static_cast<int>(MIN_VELOCITY * 1000.0)) / 1000.0;

                // Adjust size range as needed
                particle.size = rand() % (maxParticleSize - minParticleSize) + minParticleSize;
                particle.life = maxParticleLife; // Reset the lifetime
                particle.alpha = 150;  // Reset the alpha channel
            }
        } 
    } 
}

void SandParticles::Render() {
    if (particlesActive) {
        for (const auto& particle : particles) {
            int particleX = static_cast<int>(particle.x + Camera::pos.x);
            int particleY = static_cast<int>(particle.y);

            // Modified to use SDL_Rect
            SDL_Rect particleRect = { particleX, particleY, particle.size, particle.size };
            SDL_SetRenderDrawColor(renderer, particleColor.r, particleColor.g, particleColor.b, particle.alpha);
            SDL_RenderFillRect(renderer, &particleRect);
        }
    }
}

void SandParticles::initializeParticles() {
    particles.clear();
    // Adjust parameters as needed for the simulation
    for (int i = 0; i < numParticles; ++i) {
        Particle particle;
        particle.x = rand() % screenWidth;
        particle.y = rand() % screenHeight;
        particle.speed = rand() % 3 + 1; // Adjust speed range as needed
        particle.size = rand() % (maxParticleSize - minParticleSize) + minParticleSize; // Adjust size range as needed
        particle.life = static_cast<float>(rand() % static_cast<int>(maxParticleLife * 1000)) / 1000.0f; // Random initial lifetime
        particle.alpha = 150;  // Initialize the alpha channel
        particles.push_back(particle); 
    }
}