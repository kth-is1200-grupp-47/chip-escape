#include "game/state.h"
#include "hw/input.h"
#include "hw/eeprom.h"
#include "hw/display.h"

USE_IMAGE(logo);
USE_IMAGE(icons);

USE_LEVEL(level1);

const char* options[] = { "Start Game", "Highscores", "Select Level" };
int selected_option;

/* Animation for arrows */
int left_arrow_offset;
int right_arrow_offset;
int eeprom;

void main_menu_load() {
	selected_option = 0;
	left_arrow_offset = 0;
	right_arrow_offset = 0;
}

void main_menu_update(int frame) {
	if(input_get_btns_pressed() & BUTTON_LEFT) {
		selected_option--;
		left_arrow_offset = 12;
	}
	if(input_get_btns_pressed() & BUTTON_RIGHT) {
		selected_option++;
		right_arrow_offset = 12;
	}

	if(left_arrow_offset > 0) {
		left_arrow_offset--;
	}
	if(right_arrow_offset > 0) {
		right_arrow_offset--;
	}

	if(selected_option < 0) {
		selected_option = (sizeof(options)/sizeof(const char*)) - 1;
	}
	if(selected_option > (sizeof(options)/sizeof(const char*)) - 1) {
		selected_option = 0;
	}

	if(input_get_btns_pressed() & BUTTON_ACTION){
		switch(selected_option) {
			case 0:
				switch_state(STATE_SETUP_GAME, level_level1);
				break;

			case 1:
				switch_state(STATE_HIGHSCORE_LIST, 0);
				break;

			case 2:
				switch_state(STATE_SELECT_LEVEL, 0);
				break;
		}
	}
}

void main_menu_draw() {
	display_draw_image(image_logo, 0, 0, 0);

	const char* longest_opt = options[2];

	int opt_startx = DISPLAY_WIDTH / 2 - strlen(longest_opt) * FONT_CHAR_WIDTH / 2;
	int opt_endx = opt_startx + strlen(longest_opt) * FONT_CHAR_WIDTH;

	/* Left arrow */
	display_draw_image_region(image_icons, opt_startx - 10 - left_arrow_offset / 4, DISPLAY_HEIGHT - 8, 7, 0, 6, 6, 0);
	/* Right arrow */
	display_draw_image_region(image_icons, opt_endx + 4 + right_arrow_offset / 4, DISPLAY_HEIGHT - 8, 7 + 6, 0, 6, 6, 0);
	/* Text */
	const char* opt = options[selected_option];
	display_draw_text(opt, DISPLAY_WIDTH / 2 - strlen(opt) * FONT_CHAR_WIDTH / 2, DISPLAY_HEIGHT - 9, 0);
}