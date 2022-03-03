#include "game/state.h"

#include "hw/display.h"
#include "hw/input.h"

USE_LEVEL(level1);
USE_LEVEL(level2);
USE_LEVEL(level3);

void select_level_load() {
}

void select_level_update(int framenum) {
	if(input_get_btns_pressed() & BUTTON_CHIPKIT1) {
		switch_state(STATE_SETUP_GAME, level_level1);
	}
	if(input_get_btns_pressed() & BUTTON_CHIPKIT2) {
		switch_state(STATE_SETUP_GAME, level_level2);
	}
	if(input_get_btns_pressed() & BUTTON_CHIPKIT3) {
		switch_state(STATE_SETUP_GAME, level_level3);
	}
	if(input_get_btns_pressed() & BUTTON_CHIPKIT4) {
		switch_state(STATE_MAIN_MENU, 0);
	}
}

const char* lines[] = { "[BTN1] Level 1", "[BTN2] Level 2", "[BTN3] Level 3", "[BTN4] Back" };

void select_level_draw() {
	int y = DISPLAY_HEIGHT / 2 - FONT_CHAR_HEIGHT * 2;

	for(int i = 0; i < sizeof(lines)/sizeof(const char*); i++) {
		const char* line = lines[i];
		display_draw_text(line, DISPLAY_WIDTH / 2 - strlen(line) * FONT_CHAR_WIDTH / 2, y, 0);
		y += FONT_CHAR_HEIGHT;
	}
}