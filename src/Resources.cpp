#include "Resources.h"
#include <algorithm> // Include for std::for_each
#include <iostream> // Include for std::cout

std::unordered_map<std::string, SDL_Texture*> Resources::imageTable;
std::unordered_map<std::string, Mix_Music*> Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk*> Resources::soundTable;
std::unordered_map<std::string, TTF_Font *> Resources::fontTable;

SDL_Texture* Resources::GetImage(std::string file) {
    SDL_Texture* texture = nullptr;
    
    try {
        auto it = Resources::imageTable.find(file);
        
        if (it != Resources::imageTable.end()) {
            return it->second;
        }

        std::cout << "Loading a new image file... (" << file << ")" << std::endl;
        texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());

        if (texture != nullptr) {
            Resources::imageTable.insert({file, texture});
            std::cout << "Loading done! (" << file << ")" << std::endl;
            return texture;
        } else {
            throw std::runtime_error("Failed to load image file: " + file);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        // Error handling: Quit the game or take appropriate actions in case of initialization failure.
        // In this example, we just log the error to the console.
        return nullptr;
    }
}

void Resources::ClearImages() {
    std::for_each(imageTable.begin(), imageTable.end(),
        [](std::unordered_map<std::string, SDL_Texture*>::value_type& pair) {
            SDL_DestroyTexture(pair.second);
        });
    imageTable.clear();
    std::cout << "---All images erased---" << std::endl;
}


Mix_Music* Resources::GetMusic(std::string file) {
    Mix_Music* music = nullptr;

    try {
        auto it = Resources::musicTable.find(file);
        if (it == Resources::musicTable.end()) {
            std::cout << "Loading a new music file... (" << file << ")" << std::endl;
            music = Mix_LoadMUS(file.c_str());

            if (music != nullptr) {
                Resources::musicTable.insert({file, music});
                std::cout << "Loading done! (" << file << ")" << std::endl;
                return music;
            } else {
                throw std::runtime_error("Failed to load music file: " + file);
            }
        }
        return it->second;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        // Error handling: Quit the game or take appropriate actions in case of initialization failure.
        // In this example, we just log the error to the console.
        return nullptr;
    }
}

void Resources::ClearMusics() {
    std::for_each(musicTable.begin(), musicTable.end(),
        [](std::unordered_map<std::string, Mix_Music*>::value_type& pair) {
            Mix_FreeMusic(pair.second);
        });
    musicTable.clear();
    std::cout << "---All musics erased---" << std::endl;
}

Mix_Chunk* Resources::GetSound(std::string file) {
    Mix_Chunk* chunk = nullptr;

    try {
        auto it = Resources::soundTable.find(file);
        if (it == Resources::soundTable.end()) {
            std::cout << "Loading a new sound file... (" << file << ")" << std::endl;
            chunk = Mix_LoadWAV(file.c_str());

            if (chunk != nullptr) {
                Resources::soundTable.insert({file, chunk});
                std::cout << "Loading done! (" << file << ")" << std::endl;
                return chunk;
            } else {
                throw std::runtime_error("Failed to load sound file: " + file);
            }
        }
        return it->second;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        // Error handling: Quit the game or take appropriate actions in case of initialization failure.
        // In this example, we just log the error to the console.
        return nullptr;
    }
}

void Resources::ClearSounds() {
    std::for_each(soundTable.begin(), soundTable.end(),
        [](std::unordered_map<std::string, Mix_Chunk*>::value_type& pair) {
            Mix_FreeChunk(pair.second);
        });
    soundTable.clear();
    std::cout << "---All sounds erased---" << std::endl;
}

TTF_Font* Resources::GetFont(std::string file, int fontSize) {
    TTF_Font *font = nullptr;

    try {
        std::string key = file + std::to_string(fontSize); // Inclui o tamanho da fonte no nome da chave
        auto it = Resources::fontTable.find(key);

        if (it == Resources::fontTable.end()) {
            std::cout << "Loading a new font file... (" << file << ", Size: " << fontSize << ")" << std::endl;
            font = TTF_OpenFont(file.c_str(), fontSize);

            if (font != nullptr) {
                Resources::fontTable.insert({key, font});
                std::cout << "Loading done! (" << file << ", Size: " << fontSize << ")" << std::endl;
                return font;
            } else {
                throw std::runtime_error("Failed to load font file: " + file);
            }
        }
        return it->second;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        // Error handling: Quit the game or take appropriate actions in case of initialization failure.
        // In this example, we just log the error to the console.
        return nullptr;
    }
}

void Resources::ClearFonts() {
    std::for_each(fontTable.begin(), fontTable.end(),
        [](const std::pair<std::string, TTF_Font*>& pair) {
            TTF_CloseFont(pair.second);
        });
    fontTable.clear();
    std::cout << "---All fonts erased---" << std::endl;
}
