#include "GameData.h"
#include "Mother.h"
#include "Daughter.h"

bool GameData::playerTurn = true; // victory condition
int  GameData::hp = 2; 
int  GameData::hpMax = MOTHER_HP; 
int  GameData::hpCorrupted = 0; 
int  GameData::life = DAUGHTER_HP; 
int  GameData::lifeMax = DAUGHTER_HP; 
int  GameData::npLevel = 0; //init 