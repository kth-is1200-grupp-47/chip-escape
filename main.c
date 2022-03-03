#include <pic32mx.h>
#include <math.h>

#include "debug.h"
#include "data/macros.h"

#include "hw/display.h"
#include "hw/eeprom.h"
#include "hw/input.h"
#include "hw/interrupts.h"
#include "hw/spi.h"
#include "hw/timer.h"

#include "game/state.h"

USE_LEVEL(level1);

/* The amount of time a frame must take to keep a consistent frame rate. */
#define MIN_FRAME_TIME 16

int main() {
	/* Initialize the Serial Peripheral Interface, for communicating with display. */
	spi_init();

	/* Initialize interrupts and timer. */
	timer_init();
	init_interrupt();

	/* Initialize and clear OLED display. */
	display_init();

	/* Initialize input devices. */
	input_init();

	/* All devices initialized so visual crashes can be enabled. */
	debug_enable_visual_crashes();

	/* The time at which the current frame started. */
	int start_time = timer_time();
	/* The time it took to process the last frame. */
	int elapsed_time = 0;
	/* The current frame number (reset when switching states) */
	int frame = 0;
	/* Last state (for resetting frame) */
	GameState last_state = STATE_MAIN_MENU;

	/* Start game at main menu */
	switch_state(STATE_MAIN_MENU, 0);

	while(true) {
		display_clear(false);
		input_poll();

		switch(current_state) {
			case STATE_MAIN_MENU:
				main_menu_update(frame);
				main_menu_draw();
				break;

			case STATE_LEVEL:
				level_update(frame);
				level_draw();
				break;

			case STATE_HIGHSCORE_LIST:
				high_score_update(frame);
				high_score_draw();
				break;
		}

		/* Debugging functions */
		debug_on_frame(elapsed_time);
		debug_draw_overlay();

		if(timer_time() - start_time < MIN_FRAME_TIME) {
			timer_wait(1); /* align to millisecond */
		}

		elapsed_time = timer_time() - start_time;
		/* If this frame took less than 16 ms we need to sleep */
		timer_wait(MIN_FRAME_TIME - elapsed_time);

		/* Reset frame number when switching states */
		if(last_state != current_state) {
			frame = 0;
			last_state = current_state;
		}
		frame++;

		/* We start the next frame by sending the buffer to the display */
		start_time = timer_time();
		display_send_buffer();
	}

	return 0;
}