#include <pic32mx.h>
#include <math.h>

#include "data/macros.h"

#include "hw/display.h"
#include "hw/eeprom.h"
#include "hw/inputs.h"
#include "hw/interrupts.h"
#include "hw/spi.h"
#include "hw/timer.h"

#include "game/state.h"

/* The amount of time a frame must take to keep a consistent frame rate. */
#define MIN_FRAME_TIME 16

USE_IMAGE(test);

int main() {
	/* Initialize the Serial Peripheral Interface, for communicating with display. */
	spi_init();
	/* Init EEPROM I2C. */
	init_i2c();

	/* Initialize interrupts and timer. */
	timer_init();
	init_interrupt();

	/* Initialize and clear OLED display. */
	display_init();

	/* Initialize input devices. */
	input_init();

	/* The time at which the current frame started. */
	int start_time = timer_time();
	/* The time it took to process the last frame. */
	int elapsed_time = 0;
	/* The current frame number */
	int frame = 0;

	while(true) {
		display_clear(false);

		switch(current_state) {
			case STATE_MAIN_MENU:
				main_menu_update(frame);
				main_menu_draw();
				break;

			/*case STATE_LEVEL:
				level_update(frame);
				level_draw();
				break;*/
		}

		elapsed_time = timer_time() - start_time;

		/* If this frame took less than 16 ms we need to sleep */
		if(elapsed_time < MIN_FRAME_TIME) {
			int time_left = MIN_FRAME_TIME - elapsed_time;

			timer_wait(1); /* align to start of millisecond */
			time_left--;

			timer_wait(time_left);
		}

		/* We start the next frame by sending the buffer to the display */
		start_time = timer_time();
		frame++;

		display_send_buffer();
	}

	return 0;
}