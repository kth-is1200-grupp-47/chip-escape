/*
 * This file contains difficulty levels for the player
 * Written by Hannes Mann
 */

#ifndef DIFFICULTY_H
#define DIFFICULTY_H

/* Easiest difficulty - 5 lives */
#define DIFFICULTY_EASY 0
/* Normal difficulty - 3 lives */
#define DIFFICULTY_NORMAL 1
/* Nightmare difficulty - 1 lives */
#define DIFFICULTY_NIGHTMARE 2

extern const char* difficulty_names[3];
extern int difficulty_life_count[3];

extern int current_difficulty;

#endif