#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Game.h"
#include "CombatState.h"
#include "Enemies.h"
#include "Skill.h"

#include <iostream>
#include <exception>

int main(int argc, char** argv) {
    try {
        // ======================== Game happens here ===============================
        Game& game = Game::GetInstance();

        // Populate the map with skill information during initialization.
        Skill::InitializeSkillInfoMap();

        // Populate the map with enemies information during initialization.
        Enemies::InitializeEnemyInfoMap();

        // ==========Enemies array==============
        GameObject *enemy = new GameObject();
        std::vector<std::shared_ptr<Enemies>> enemiesArray;
        enemiesArray.push_back(std::make_shared<Enemies>(*enemy, Enemies::ENEMY1));
        enemiesArray.push_back(std::make_shared<Enemies>(*enemy, Enemies::ENEMY2));
        enemiesArray.push_back(std::make_shared<Enemies>(*enemy, Enemies::ENEMY3));
        enemiesArray.push_back(std::make_shared<Enemies>(*enemy, Enemies::ENEMY3));

        CombatState* initialState = new CombatState(enemiesArray); // Pass the enemiesArray
        
        game.Push(initialState);

        game.Run();
        // ==========================================================================
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1; // Return a non-zero error code to indicate an error.
    }

    return 0;
}

