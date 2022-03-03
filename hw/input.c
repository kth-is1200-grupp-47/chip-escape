#include <stdint.h>
#include <pic32mx.h>
#include "hw/input.h"

uint8_t switches;

/* Save buttons from last frame */
uint16_t last_buttons;
uint16_t buttons;

/****************************************
 * This function initializes the inputs *
 * Written by Botan Botani              *
 * *************************************/
void input_chipkit_init(){
    /* Initialization of buttons 2-4 (bits 5-7 of port D) and switch 1-4 (bits 8-11 of port D) as input*/
    TRISD |= 0xFE0;

    /* Initialization of button 1 (bit 1 of port F) as input */
    TRISF |= 0x2;
}

void input_init() {
	input_chipkit_init();
	/* TODO: input_nes_init(); */
}

/* Poll from I/O shield */
void input_chipkit_poll() {
	/* Read values once */
	uint32_t portd_reg = PORTD;
	uint32_t portf_reg  = PORTF;

	/* Status of switch 1 */
	switches |= (portd_reg & (1 << 8)) >> 8;
    /* Status of switch 2 */
	switches |= (portd_reg & (1 << 9)) >> 8;
    /* Status of switch 3 */
	switches |= (portd_reg & (1 << 10)) >> 8;
    /* Status of switch 4 */
	switches |= (portd_reg & (1 << 11)) >> 8;

	/* Map ChipKIT registers to buttons defined in input.h */
	if(portd_reg & (1 << 7)) {
		buttons |= BUTTON_LEFT;
		buttons |= BUTTON_CHIPKIT4;
	}
	if(portd_reg & (1 << 6)) {
		buttons |= BUTTON_RIGHT;
		buttons |= BUTTON_CHIPKIT3;
	}
	if(portd_reg & (1 << 5)) {
		buttons |= BUTTON_JUMP;
		buttons |= BUTTON_MENU_UP;
		buttons |= BUTTON_CHIPKIT2;
	}
	if(PORTF & (1 << 1)) {
		buttons |= BUTTON_ACTION;
		buttons |= BUTTON_MENU_DOWN;
		buttons |= BUTTON_CHIPKIT1;
	}
}

/* Read inputs and save last, written by Botan Botani and Hannes Mann */
void input_poll() {
	last_buttons = buttons;

	switches = 0;
	buttons = 0;

	input_chipkit_poll();
	/* TODO: input_nes_poll(); */
}

uint8_t input_get_sw() {
	return switches;
}

uint16_t input_get_btns() {
	return buttons;
}

uint16_t input_get_btns_pressed() {
	return buttons & ~last_buttons;
}

uint16_t input_get_btns_released() {
	return ~buttons & last_buttons;
}