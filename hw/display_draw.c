/* display.h split into two files - this contains functions for drawing images, rectangles, text, etc */

#include "data/macros.h"
#include "hw/display.h"

/* Number of characters in the png for every line. Power of 2 so GCC can optimize calculations. */
#define CHAR_PER_LINE 16

USE_IMAGE(font_spleen);

void display_draw_rectangle(bool enable, int x, int y, uint8_t w, uint8_t h) {
	int xend = x + w;
	int yend = y + h;

	for(int yp = y; yp != yend; ++yp) {
		for(int xp = x; xp != xend; ++xp) {
			display_write_bit(enable, xp, yp);
		}
	}
}

void display_draw_image(Image image, int x, int y, DisplayOp effect) {
	display_draw_image_region(image, x, y, 0, 0, image_w(image), image_h(image), effect);
}

void display_draw_image_region(Image image, int x, int y, uint8_t rx, uint8_t ry, uint8_t rw, uint8_t rh, DisplayOp effect) {
	int xend = x + rw;
	int yend = y + rh;

	for(int yp = y; yp != yend; ++yp) {
		for(int xp = x; xp != xend; ++xp) {
			switch(effect) {
				/* OR */
				case OP_OVERLAY:
					display_write_bit(display_get_bit(xp, yp) | image_bit(image, xp - x + rx, yp - y + ry), xp, yp);
					break;

				/* XOR */
				case OP_INVERT:
					display_write_bit(display_get_bit(xp, yp) ^ image_bit(image, xp - x + rx, yp - y + ry), xp, yp);
					break;

				/* NOT */
				case OP_DISABLE:
					display_write_bit(display_get_bit(xp, yp) & ~image_bit(image, xp - x + rx, yp - y + ry), xp, yp);
					break;

				default:
					display_write_bit(image_bit(image, xp - x + rx, yp - y + ry), xp, yp);
			}
		}
	}
}

void display_draw_text(const char* text, int x, int y, DisplayOp effect) {
	int xstart = x;

	/* Text can only use OR XOR NOT */
	if(!effect) {
		effect = OP_OVERLAY;
	}

	char c = *text++;
	while(c) {
		if(c == '\n') {
			x = xstart;
			y += FONT_CHAR_HEIGHT;
		}
		else {
			/* We include ASCII region 33-126 */
			if(c >= '!' && c <= '~') {
				uint8_t line = (c - '!') / CHAR_PER_LINE;
				char first_char_for_line = '!' + (line * CHAR_PER_LINE);

				uint8_t char_pos_x = (c - first_char_for_line) * FONT_CHAR_WIDTH;
				uint8_t char_pos_y = line * FONT_CHAR_HEIGHT;

				display_draw_image_region(image_font_spleen, x, y, char_pos_x, char_pos_y, FONT_CHAR_WIDTH, FONT_CHAR_HEIGHT, effect);
			}

			x += FONT_CHAR_WIDTH;
		}

		c = *text++;
	}
}