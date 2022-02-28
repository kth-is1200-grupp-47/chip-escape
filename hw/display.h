/*
 * This file contains functions for interacting with the ChipKIT OLED display.
 * Written by Hannes Mann
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdio.h>

#include "data/macros.h"

#define DISPLAY_WIDTH 128
/* Memory pages */
#define DISPLAY_PAGES 4
/* Pixels in memory page column */
#define DISPLAY_PAGE_HEIGHT 8
#define DISPLAY_HEIGHT (DISPLAY_PAGES * DISPLAY_PAGE_HEIGHT)

/* Width and height of characters in the game font (spleen) */
#define FONT_CHAR_WIDTH 5
#define FONT_CHAR_HEIGHT 8

/* Write enabled bits only as white pixels */
#define OP_OVERLAY 1
/* Write enabled bits only as black pixels */
#define OP_DISABLE 2
/* XOR with display buffer */
#define OP_INVERT 3

/* Type of display operation */
typedef uint8_t DisplayOp;

/* 1-bit array with the same format as OLED display containing the next frame */
extern uint8_t display_buffer[DISPLAY_WIDTH * DISPLAY_PAGES];

/* Initialize the OLED display. Assumes SPI is initialized. */
void display_init();

/* Clear all bits of the display */
static void display_clear(bool enable) {
	memset(display_buffer, enable ? 0xff : 0, sizeof(display_buffer));
}

/* Update the display with the contents of display_buffer (without clearing the buffer). */
void display_send_buffer();

/* Get bit in the display buffer at the specified coordinates. */
static bool display_get_bit(uint16_t x, uint16_t y) {
	uint8_t page = y / DISPLAY_PAGE_HEIGHT;
	uint8_t bit = y - (page * DISPLAY_PAGE_HEIGHT);

	return display_buffer[page * DISPLAY_WIDTH + x] & (1 << bit);
}

/* Write a single bit to the display buffer at the specified coordinates. */
static void display_write_bit(bool enable, uint16_t x, uint16_t y) {
	uint8_t page = y / DISPLAY_PAGE_HEIGHT;
	uint8_t bit = y - (page * DISPLAY_PAGE_HEIGHT);

	/* Allow writes outside the display area but do nothing (bitwise check for speed). */
	if(x & DISPLAY_WIDTH || y & DISPLAY_HEIGHT) {
		return;
	}

	if(enable) {
		display_buffer[page * DISPLAY_WIDTH + x] |= 1 << bit;
	}
	else {
		display_buffer[page * DISPLAY_WIDTH + x] &= ~(1 << bit);
	}
}

/* Draw a rectangle on the display buffer at the specified coordinates. */
void display_draw_rectangle(bool enable, int x, int y, uint8_t w, uint8_t h);

/* Draw an image on the display buffer at the specified coordinates. */
void display_draw_image(Image image, int x, int y, DisplayOp effect /* 0 or OP_X */);

/* Draw part of an image on the display buffer at the specified coordinates. */
void display_draw_image_region(Image image, int x, int y, uint8_t rx, uint8_t ry, uint8_t rw, uint8_t rh, DisplayOp effect /* 0 or OP_X */);

/* Draw text on the display buffer at the specified coordinates. */
void display_draw_text(const char* text, int x, int y, DisplayOp effect /* 0 or OP_DISABLE||OP_INVERT */);

#endif