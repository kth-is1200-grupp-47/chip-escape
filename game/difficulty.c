#include "difficulty.h"

const char* difficulty_names[3] = { "Easy", "Normal", "Nightmare" };
int difficulty_life_count[3] = { 12, 8, 1 };

int current_difficulty = DIFFICULTY_NORMAL;