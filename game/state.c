#include "state.h"

GameState current_state = 0;
GameState previous_state = 0;

void switch_state(GameState new_state, const void* load_data) {
	previous_state = current_state;
	current_state = new_state;

	/* Run load function */
	switch(current_state) {
		case STATE_MAIN_MENU:
			main_menu_load();
			break;

		case STATE_LEVEL:
			level_load((Level)load_data);
			break;

		case STATE_HIGHSCORE_LIST:
			high_score_load((const int*)load_data);
			break;
	}
}