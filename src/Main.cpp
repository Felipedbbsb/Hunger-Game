#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Game.h"
#include "CombatState.h"
#include "Map.h"
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

        Skill::InitializeSkills();

        // Populate the map with enemies information during initialization.
        Enemies::InitializeEnemyInfoMap();
 
        // ==========Enemies array==============
        /*
        std::vector<Enemies::EnemyId> enemiesArray = { Enemies::ENEMY1, 
                                                       Enemies::ENEMY2, 
                                                       Enemies::ENEMY3 };
 
        CombatState* initialState = new CombatState(enemiesArray, COMBAT_IMAGE); // Pass the enemiesArray
        */

        Map* initialState = new Map();    
        game.Push(initialState);
 
        game.Run();
        // ==========================================================================
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1; // Return a non-zero error code to indicate an error.
    }

    return 0;
}

