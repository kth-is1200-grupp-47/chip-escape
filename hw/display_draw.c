/* display.h split into two files - this contains functions for drawing images, rectangles, text, etc */

#include "data/macros.h"
#include "hw/display.h"

/* Number of characters in the png for every line */
#define CHAR_PER_LINE 20

USE_IMAGE(font_spleen);

void display_draw_rectangle(bool enable, int x, int y, int w, int h) {
	for(int yp = y; yp < y + h; yp++) {
		for(int xp = x; xp < x + w; xp++) {
			display_write_bit(enable, xp, yp);
		}
	}
}

void display_draw_image(Image image, int x, int y) {
	display_draw_image_region(image, x, y, 0, 0, width_of(image), height_of(image));
}

void display_overlay_image(Image image, int x, int y) {
	display_overlay_image_region(image, x, y, 0, 0, width_of(image), height_of(image));
}

void display_draw_image_region(Image image, int x, int y, int rx, int ry, int rw, int rh) {
	for(int yp = y; yp < y + rh; yp++) {
		for(int xp = x; xp < x + rw; xp++) {
			display_write_bit(image_bit(image, xp - x + rx, yp - y + ry), xp, yp);
		}
	}
}

void display_overlay_image_region(Image image, int x, int y, int rx, int ry, int rw, int rh) {
	for(int yp = y; yp < y + rh; yp++) {
		for(int xp = x; xp < x + rw; xp++) {
			if(image_bit(image, xp - x + rx, yp - y + ry)) {
				display_write_bit(true, xp, yp);
			}
		}
	}
}

void display_draw_text(const char* text, int x, int y) {
	int xstart = x;

	while(true) {
		char c = *text++;

		switch(c) {
			/* New line */
			case '\n':
				x = xstart;
				y += FONT_CHAR_HEIGHT;
				break;

			/* End of string */
			case '\0':
				return;

			/* Other character or space */
			default:
				/* We include ASCII region 33-126 */
				if(c >= '!' && c <= '~') {
					uint8_t line = (c - '!') / CHAR_PER_LINE;
					char first_char_for_line = '!' + (line * CHAR_PER_LINE);

					uint8_t char_pos_x = (c - first_char_for_line) * FONT_CHAR_WIDTH;
					uint8_t char_pos_y = line * FONT_CHAR_HEIGHT;

					display_overlay_image_region(
						image_font_spleen,
						x, y,
						char_pos_x, char_pos_y,
						FONT_CHAR_WIDTH, FONT_CHAR_HEIGHT
					);
				}

				x += FONT_CHAR_WIDTH;
		}
	}
}