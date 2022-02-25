#ifndef STATE_H
#define STATE_H

/* Player is in the main menu */
#define STATE_MAIN_MENU 0
/* Player is in a level */
#define STATE_LEVEL 1
/* Player is at the highscore list */
#define STATE_HIGHSCORE_LIST 2

/* The current state of the game */
extern int current_state;

/*
 * "load()" runs the first time the game switches to a new state
 * "update()" runs every frame before drawing
 * "draw()" runs at the end of every frame
 */

void main_menu_load();
void main_menu_update(int framenum);
void main_menu_draw();

void level_load(); /* TODO: choose level */
void level_update(int framenum);
void level_draw();

/* Switch to a different state and run load function */
void switch_state(int new_state);

#endif