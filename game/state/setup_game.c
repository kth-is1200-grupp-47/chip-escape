#include "game/difficulty.h"
#include "game/state.h"

#include "hw/display.h"
#include "hw/input.h"

USE_IMAGE(icons);
USE_LEVEL(level1);

/* Animation for arrows */
int left_arrow_offset;
int right_arrow_offset;

void setup_game_load() {
	current_difficulty = DIFFICULTY_NORMAL;
	left_arrow_offset = 0;
	right_arrow_offset = 0;
}

void setup_game_update(int framenum) {
	if(input_get_btns_pressed() & BUTTON_LEFT) {
		current_difficulty--;
		left_arrow_offset = 12;
	}
	if(input_get_btns_pressed() & BUTTON_RIGHT) {
		current_difficulty++;
		right_arrow_offset = 12;
	}
	if(input_get_btns_pressed() & BUTTON_ACTION) {
		switch_state(STATE_LEVEL, level_level1);
	}

	if(left_arrow_offset > 0) {
		left_arrow_offset--;
	}
	if(right_arrow_offset > 0) {
		right_arrow_offset--;
	}

	/* Wrap around after nightmare */
	if(current_difficulty < DIFFICULTY_EASY) {
		current_difficulty = DIFFICULTY_NIGHTMARE;
	}
	if(current_difficulty > DIFFICULTY_NIGHTMARE) {
		current_difficulty = DIFFICULTY_EASY;
	}
}
void setup_game_draw() {
	display_draw_text("Difficulty", DISPLAY_WIDTH / 2 - strlen("Difficulty") * FONT_CHAR_WIDTH / 2, 0, 0);

	const char* df = difficulty_names[current_difficulty];
	display_draw_text(df, DISPLAY_WIDTH / 2 - strlen(df) * FONT_CHAR_WIDTH / 2, FONT_CHAR_HEIGHT + 2, 0);

	/* Always draw arrows in the same position */
	const char* longest_df = difficulty_names[DIFFICULTY_NIGHTMARE];

	int df_startx = DISPLAY_WIDTH / 2 - strlen(longest_df) * FONT_CHAR_WIDTH / 2;
	int df_endx = df_startx + strlen(longest_df) * FONT_CHAR_WIDTH;

	/* Left arrow */
	display_draw_image_region(image_icons, df_startx - 10 - left_arrow_offset / 4, FONT_CHAR_HEIGHT + 3, 7, 0, 6, 6, 0);
	/* Right arrow */
	display_draw_image_region(image_icons, df_endx + 4 + right_arrow_offset / 4, FONT_CHAR_HEIGHT + 3, 7 + 6, 0, 6, 6, 0);

	const char* label = "[BTN1] Continue";
	display_draw_text(label, DISPLAY_WIDTH / 2 - FONT_CHAR_WIDTH * strlen(label) / 2, DISPLAY_HEIGHT - 9, 0);
}