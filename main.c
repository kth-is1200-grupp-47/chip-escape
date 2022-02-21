#include <pic32mx.h>
#include <math.h>

#include "data/macros.h"

#include "hw/display.h"
#include "hw/spi.h"

USE_IMAGE(test);

int main() {
	/* Initialize the Serial Peripheral Interface, for communicating with display. */
	spi_initialize();
	/* Initialize and clear OLED display. */
	display_initialize();

	int t = 0;
	while(true) {
		uint8_t bit_pos = DISPLAY_WIDTH - 1;
		bit_pos *= (sin(t / 20.0) + 1.0) / 2.0;

		display_write_bit(true, bit_pos, DISPLAY_HEIGHT / 2);
		display_send_buffer();

		/* TODO: fix drawing images */

		for(int i = 0; i < 4711 * 20; i++) {}
		t++;
	}

	return 0;
}

void user_isr() {
	/* TODO: Interrupt handler */
}