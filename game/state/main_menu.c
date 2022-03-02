#include "game/state.h"
#include "hw/inputs.h"

USE_LEVEL(test);

void main_menu_load() {

}

/************************************************************
 * If the game is in the main menu and buttons are pressed  *
 * options are marked temporarily                           *
 * Written by Botan Botani                                  *
 ************************************************************/
void main_menu_update(int frame) {
	int pushed_buttons = getBtns();

	/* High score */
	if(pushed_buttons & 0b0010){
		switch_state(STATE_HIGHSCORE_LIST, 0);
	}

	/* Play */
	if(pushed_buttons & 0b1000){
		switch_state(STATE_LEVEL, level_test);
	}
}

void main_menu_draw() {
}