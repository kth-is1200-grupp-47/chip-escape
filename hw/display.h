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

/* 1-bit array with the same format as OLED display containing the next frame */
extern uint8_t display_buffer[DISPLAY_WIDTH * DISPLAY_PAGES];

/* Initialize the OLED display. Assumes SPI is initialized. */
void display_init();

/* Clear all bits of the display */
static inline void display_clear(bool enable) {
	memset(display_buffer, enable ? 0xff : 0, sizeof(display_buffer));
}

/* Write an image to the display buffer at the specified coordinates. */
void display_draw_image(Image image, uint8_t x, uint8_t y);

/* Write all enabled bits of an image to the display buffer at the specified coordinates. */
void display_overlay_image(Image image, uint8_t x, uint8_t y);

/* Write a single bit to the display buffer at the specified coordinates. */
void display_write_bit(bool enable, uint8_t x, uint8_t y);

/* Update the display with the contents of display_buffer (without clearing the buffer). */
void display_send_buffer();

#endif