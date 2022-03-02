/*
 * The "game state" indicates what the game is doing (in a menu? in a level? etc.)
 * Written by Hannes Mann
 */

#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include "data/macros.h"

/* Player is in the main menu */
#define STATE_MAIN_MENU 0
/* Player is in a level */
#define STATE_LEVEL 1
/* Player is at the highscore list */
#define STATE_HIGHSCORE_LIST 2

typedef uint8_t GameState;

/* The current state of the game */
extern GameState current_state;

/*
 * "load()" runs the first time the game switches to a new state
 * "update()" runs every frame before drawing
 * "draw()" runs at the end of every frame
 */

void main_menu_load();
void main_menu_update(int framenum);
void main_menu_draw();

void level_load(Level data);
void level_update(int framenum);
void level_draw();

void high_score_load(const int* data);
void high_score_update(int framenum);
void high_score_draw();

/* Switch to a different state and run load function */
void switch_state(GameState new_state, const void* load_data);

#endif