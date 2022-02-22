/* Written by Hannes Mann */

#include <assert.h>
#include <stdint.h>

#include "data/macros.h"

bool image_bit(Image image, int x, int y) {
	/* Wrap around if coordinates are outside the image. */
	x = x % width_of(image);
	y = y % height_of(image);

	/* Absolute bit position starts at position 0, the most-significant bit in the entire array */
	int bit = (y * height_of(image)) + x;
	int byte = bit / 8;

	/* MSB = first */
	uint8_t offset = 7 - bit % 8;
	uint8_t bitmask = 1 << offset;

	return (image[byte] & bitmask) >> offset;
}