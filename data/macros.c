/* Written by Hannes Mann */

#include <assert.h>
#include <stdint.h>

#include "data/macros.h"

bool image_bit(Image image, uint8_t x, uint8_t y) {
	/* Absolute bit position starts at position 0, the most-significant bit in the entire array */
	int bit = (y * height_of(image)) + x;
	int byte = bit / 8;

	/* MSB = first */
	uint8_t offset = 7 - bit % 8;
	uint8_t bitmask = 1 << offset;

	return (image[byte] & bitmask) >> offset;
}