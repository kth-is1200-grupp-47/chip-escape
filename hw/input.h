/* Written by Botan Botani (more functions added by Hannes Mann) */

#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <pic32mx.h>
#include <stdbool.h>

/* Button for left direction (ChipKIT = BTN4, NES = ???) */
#define BUTTON_LEFT 1
/* Button for right direction (ChipKIT = BTN3, NES = ???) */
#define BUTTON_RIGHT 2
/* Button for jumping (ChipKIT = BTN2, NES = ???) */
#define BUTTON_JUMP 4
/* Button for other actions (ChipKIT = BTN1, NES = ???) */
#define BUTTON_ACTION 8

/* Button 1 on ChipKIT only */
#define BUTTON_CHIPKIT1 16
/* Button 2 on ChipKIT only */
#define BUTTON_CHIPKIT2 32
/* Button 3 on ChipKIT only */
#define BUTTON_CHIPKIT3 64
/* Button 4 on ChipKIT only */
#define BUTTON_CHIPKIT4 128

/* Runs at game start */
void input_init();
/* Runs at beginning of every frame */
void input_poll();

/* Four bits for switches on ChipKIT */
uint8_t input_get_sw();

/* button - BUTTON_XXX */
uint16_t input_get_btns();
uint16_t input_get_btns_pressed() /* current && !last */;
uint16_t input_get_btns_released() /* !current && last */;

#endif