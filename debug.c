#include <stdbool.h>

#include "debug.h"

#include "hw/display.h"
#include "hw/inputs.h"
#include "hw/timer.h"

bool visual_crash_enabled = false;

void debug_enable_visual_crashes() {
	visual_crash_enabled = true;
}

/* Define __assert_fail for GCC */
void __assert_fail(const char* assertion, const char* file, unsigned int line, const char* function) {
	crash(assertion);
}

void crash(const char* reason) {
	/* If display is initialized show the crash reason */
	if(visual_crash_enabled) {
		display_clear(false);
		display_draw_text("Game crashed:", DISPLAY_WIDTH / 2 - (strlen("Game crashed:") * FONT_CHAR_WIDTH) / 2, DISPLAY_HEIGHT / 2 - FONT_CHAR_HEIGHT, 0);
		display_draw_text(reason, DISPLAY_WIDTH / 2 - (strlen(reason) * FONT_CHAR_WIDTH) / 2, DISPLAY_HEIGHT / 2, 0);
		display_send_buffer();
	}

	TRISE = 0xffffff00;
  	PORTE = 0x0;

	/* Hang and blink LEDs forever */
	while(true) {
		PORTE ^= 0xff;
		timer_wait(500);
	}
}

char fps[32];

int last_second_timestamp = 0;
int frames_since_last_second = 0;

void debug_on_frame(int elapsed_time) {
	/* SW4 = FPS overlay */
	if(getSW() & (1 << 3)) {
		if(last_second_timestamp == 0) {
			last_second_timestamp = timer_time();
			memset(fps, 0, 32);
		}

		/* Count the number of frames in a second */
		if(timer_time() - last_second_timestamp >= 1000) {
			sprintf(fps, "FPS: %d FT: %d ms", frames_since_last_second, elapsed_time);

			frames_since_last_second = 0;
			last_second_timestamp = timer_time();
		}

		frames_since_last_second++;
	}
}

void debug_draw_overlay() {
	/* SW4 = FPS overlay */
	if(getSW() & (1 << 3)) {
		display_draw_rectangle(true, 0, 0, DISPLAY_WIDTH - 1, FONT_CHAR_HEIGHT + 4);
		display_draw_text(fps, 2, 2, OP_INVERT);
	}
}