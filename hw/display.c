#include <pic32mx.h>
#include <string.h>

#include "data/macros.h"
#include "hw/display.h"
#include "hw/spi.h"

/* 4711 iterations per ms, taken from lab work */
#define do_and_wait(x, ms) x; for(int _delay = 0; _delay < 4711 * ms; _delay++) {}

/* From: https://digilent.com/reference/chipkit_shield_basic_io_shield/refmanual */
#define BIT_VDD_ENABLE (1 << 6)
#define BIT_VBAT_ENABLE (1 << 5)
#define BIT_SEND_DATA (1 << 4)
#define BIT_RESET (1 << 9)

#define PORT_DATA PORTF
#define PORT_DATA_SET PORTFSET
#define PORT_DATA_CLR PORTFCLR

#define PORT_RESET PORTG
#define PORT_RESET_SET PORTGSET
#define PORT_RESET_CLR PORTGCLR

#define CMD_DISPLAY_OFF 0xAE
#define CMD_DISPLAY_ON 0xAF
#define CMD_SET_PAGE 0x22

uint8_t display_buffer[DISPLAY_WIDTH * DISPLAY_PAGES];

/* Send commands to display */
void set_command_mode() {
	PORT_DATA_CLR = BIT_SEND_DATA;
}

/* Send data to display */
void set_data_mode() {
	PORT_DATA_SET = BIT_SEND_DATA;
}

void display_init() {
	/* Clear buffer to all black */
	display_clear(false);

	/* Set pins as output */
	TRISFCLR = BIT_VDD_ENABLE | BIT_VBAT_ENABLE | BIT_SEND_DATA;
	TRISGCLR = BIT_RESET;

	set_command_mode();

	/* Apply power to VDD (display logic) and ensure display is off */
	do_and_wait(PORT_DATA_CLR = BIT_VDD_ENABLE, 100);
	spi2_send(CMD_DISPLAY_OFF);

	/* Reset display */
	do_and_wait(PORT_RESET_CLR = BIT_RESET, 1);
	do_and_wait(PORT_RESET_SET = BIT_RESET, 1);

	/* Send Charge Pump and Pre-Charge Period */
	spi2_send(0x8D);
	spi2_send(0x14);
	spi2_send(0xD9);
	spi2_send(0xF1);

	/* Apply power to VBAT (the display itself) */
	do_and_wait(PORT_DATA_CLR = BIT_VBAT_ENABLE, 100);

	/* Invert the display, so the origin is at the upper left corner */
	spi2_send(0xA1);
	spi2_send(0xC8);

	/* Sequential COM configuration */
	spi2_send(0xDA);
	spi2_send(0x20);

	/* Turn the display on */
	spi2_send(CMD_DISPLAY_ON);
}

void display_draw_image(Image image, uint8_t x, uint8_t y) {
	/* Clear all bits behind the image */
	for(uint8_t xp = x; xp < x + width_of(image); xp++) {
		for(uint8_t yp = y; yp < y + height_of(image); yp++) {
			display_write_bit(false, xp, yp);
		}
	}

	display_overlay_image(image, x, y);
}

void display_overlay_image(Image image, uint8_t x, uint8_t y) {
	for(uint8_t xp = x; xp < x + width_of(image); xp++) {
		for(uint8_t yp = y; yp < y + height_of(image); yp++) {
			display_write_bit(image_bit(image, xp - x, yp - y), xp, yp);
		}
	}
}

void display_write_bit(bool enable, uint8_t x, uint8_t y) {
	uint8_t page = y / DISPLAY_PAGE_HEIGHT;
	uint16_t buffer_pos = page * DISPLAY_WIDTH + x;
	uint8_t bit = y - (page * DISPLAY_PAGE_HEIGHT);

	if(enable) {
		display_buffer[buffer_pos] |= 1 << bit;
	}
	else {
		display_buffer[buffer_pos] &= ~(1 << bit);
	}
}

void display_send_buffer() {
	uint8_t* pending_data = display_buffer;

	for(uint8_t page = 0; page < DISPLAY_PAGES; page++) {
		set_command_mode();

		/* Set page number we're writing to */
		spi2_send(CMD_SET_PAGE);
		spi2_send(page);
		/* Set low and high nibble */
		spi2_send(0x0);
		spi2_send(0x10);

		set_data_mode();

		for(uint8_t x = 0; x < DISPLAY_WIDTH; x++) {
			spi2_send(*pending_data++);
		}
	}
}