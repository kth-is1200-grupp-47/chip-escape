/* display.h split into two files - this contains functions for drawing images, rectangles, text, etc */

#include "data/macros.h"
#include "hw/display.h"

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
	while(*text++ != '\0') {
		/* TODO */
	}
}