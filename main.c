#include <pic32mx.h>
#include <math.h>

#include "data/macros.h"

#include "hw/display.h"
#include "hw/inputs.h"
#include "hw/spi.h"

USE_IMAGE(test);

int main() {
	/* Initialize the Serial Peripheral Interface, for communicating with display. */
	spi_init();
	/* Initialize and clear OLED display. */
	display_init();
	/* Initialize input devices. */
	input_init();

	char text[1024];
	memset(text, 0, 1024);
	char* last_char = text;

	int t = 0;
	while(true) {
		display_clear(false);

		uint8_t bit_pos = DISPLAY_WIDTH - 1;
		bit_pos *= (sin(t / 20.0) + 1.0) / 2.0;
		display_write_bit(true, bit_pos, DISPLAY_HEIGHT / 2);

		/* Test for draw_text */
		if(getBtns() & 1) {
			*last_char = (t % ('Z' - 'A')) + 'A';
			last_char++;
		}

		display_draw_text(text, 4, 0);
		display_draw_text("CHIP ESCAPE", 4, DISPLAY_HEIGHT - FONT_CHAR_HEIGHT);
		display_send_buffer();

		/* Placeholder until timer is done */
		for(int i = 0; i < 4711 * 20; i++) {}

		t++;
		if(t % 100 == 0) {
			memset(text, 0, 1024);
			last_char = text;
		}
	}

	return 0;
}

void user_isr() {
	/* TODO: Interrupt handler */
}