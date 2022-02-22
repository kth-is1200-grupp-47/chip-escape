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

/* 1-bit array with the same format as OLED display containing the next frame */
extern uint8_t display_buffer[DISPLAY_WIDTH * DISPLAY_PAGES];

/* Initialize the OLED display. Assumes SPI is initialized. */
void display_init();

/* Clear all bits of the display */
static inline void display_clear(bool enable) {
	memset(display_buffer, enable ? 0xff : 0, sizeof(display_buffer));
}

/* Write a single bit to the display buffer at the specified coordinates. */
void display_write_bit(bool enable, int x, int y);

/* Draw a rectangle at the specified coordinates. */
void display_draw_rectangle(bool enable, int x, int y, int w, int h);

/* Draw an image to the display buffer at the specified coordinates. */
void display_draw_image(Image image, int x, int y);

/* Write enabled bits of an image to the display buffer at the specified coordinates. */
void display_overlay_image(Image image, int x, int y);

/* Draw part of an image to the display buffer at the specified coordinates. */
void display_draw_image_region(Image image, int x, int y, int rx, int ry, int rw, int rh);

/* Write enabled bits of part of an image to the display buffer at the specified coordinates. */
void display_overlay_image_region(Image image, int x, int y, int rx, int ry, int rw, int rh);

/* Draw text at the specified coordinates. */
void display_draw_text(const char* text, int x, int y);

/* Update the display with the contents of display_buffer (without clearing the buffer). */
void display_send_buffer();

#endif