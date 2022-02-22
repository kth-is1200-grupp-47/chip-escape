/* Written by Hannes Mann */

#include <assert.h>
#include <stdint.h>

#include "data/macros.h"

bool image_bit(Image image, int x, int y) {
	/* Wrap around if coordinates are outside the image. */
	x = x % width_of(image);
	y = y % height_of(image);

	int bit = (y * width_of(image)) + x;
	/* LSB = first */
	uint8_t bitmask = 1 << (bit % 8);

	return image[bit / 8] & bitmask;
}