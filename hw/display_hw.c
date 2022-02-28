/* display.h split into two files - this contains functions for initializing and using the hardware */

#include <pic32mx.h>
#include <string.h>

#include "hw/display.h"
#include "hw/spi.h"
#include "hw/timer.h"

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
	PORT_DATA_CLR = BIT_VDD_ENABLE;
	timer_wait(100);
	spi2_send(CMD_DISPLAY_OFF);

	/* Reset display */
	PORT_RESET_CLR = BIT_RESET;
	timer_wait(1);
	PORT_RESET_SET = BIT_RESET;
	timer_wait(1);

	/* Send Charge Pump and Pre-Charge Period */
	spi2_send(0x8D);
	spi2_send(0x14);
	spi2_send(0xD9);
	spi2_send(0xF1);

	/* Apply power to VBAT (the display itself) */
	PORT_DATA_CLR = BIT_VBAT_ENABLE;
	timer_wait(100);

	/* Invert the display, so the origin is at the upper left corner */
	spi2_send(0xA1);
	spi2_send(0xC8);

	/* Sequential COM configuration */
	spi2_send(0xDA);
	spi2_send(0x20);

	/* Turn the display on */
	spi2_send(CMD_DISPLAY_ON);
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