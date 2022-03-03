#include "state.h"
#include "hw/display.h"
#include "hw/timer.h"

GameState current_state = 0;
GameState previous_state = 0;
bool switched_state = false;

void switch_state(GameState new_state, const void* load_data) {
	previous_state = current_state;
	current_state = new_state;
	switched_state = true;

	/* Run load function */
	switch(current_state) {
		case STATE_MAIN_MENU:
			main_menu_load();
			break;

		case STATE_SETUP_GAME:
			setup_game_load();
			break;

		case STATE_LEVEL:
			level_load((Level)load_data);
			break;

		case STATE_HIGHSCORE_LIST:
			high_score_load((int)load_data);
			break;
	}
}