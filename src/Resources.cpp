#include "Resources.h"

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
    int i = 0;
    for (auto it = imageTable.begin(); it != imageTable.end(); ++i) {
        std::cout << "Cleaning image memory -> (" << i + 1 << ") File: " << it->first << std::endl;
        it = imageTable.erase(it); // Correção aqui
    }
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
    int i = 0;
    for (auto it = musicTable.begin(); it != musicTable.end(); ++i) {
        std::cout << "Cleaning music memory -> (" << i + 1 << ") File: " << it->first << std::endl;
        it = musicTable.erase(it);
    }
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
    int i = 0;
    for (auto it = soundTable.begin(); it != soundTable.end(); ++i) {
        std::cout << "Cleaning sound memory -> (" << i + 1 << ") File: " << it->first << std::endl;
        it = soundTable.erase(it);
    }
    soundTable.clear();
    std::cout << "---All sounds erased---" << std::endl;

}

TTF_Font* Resources::GetFont(std::string file, int fontSize) {
    TTF_Font *font = nullptr;

    try {
        auto it = Resources::fontTable.find(file);
        if (it == Resources::fontTable.end()) {
            std::cout << "Loading a new font file... (" << file << ")" << std::endl;
            font = TTF_OpenFont(file.c_str(), fontSize);

            if (font != nullptr) {
                Resources::fontTable.insert({file, font});
                std::cout << "Loading done! (" << file << ")" << std::endl;
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
    int i = 0;
    for (auto it = fontTable.begin(); it != fontTable.end(); ++i) {
        std::cout << "Cleaning font memory -> (" << i + 1 << ") File: " << it->first << std::endl;
        it = fontTable.erase(it);
    }
    fontTable.clear();
    std::cout << "---All fonts erased---" << std::endl;
}