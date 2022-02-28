#include "state.h"

GameState current_state;

void switch_state(GameState new_state) {
	current_state = new_state;

	/* Run load function */
	switch(current_state) {
		case STATE_MAIN_MENU:
			main_menu_load();
			break;

		/*case STATE_LEVEL:
			level_load();
			break;*/
	}
}