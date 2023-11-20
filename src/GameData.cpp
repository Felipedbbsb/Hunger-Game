#include "GameData.h"
#include "Mother.h"
#include "Daughter.h"

bool GameData::playerTurn = true; // victory condition
int  GameData::hp = MOTHER_HP; 
int  GameData::hpMax = MOTHER_HP; 
int  GameData::hpCorrupted = 0; 
int  GameData::life = DAUGHTER_HP; 
int  GameData::lifeMax = DAUGHTER_HP; 
 