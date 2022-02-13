/* Written by Hannes Mann */

#include <assert.h>
#include "data/macros.h"

bool image_bit(unsigned char* image, int x, int y) {
	/* IMAGE_DATA returns a pointer at position 2 */
	unsigned char w = image[-2];
	unsigned char h = image[-1];

	/* Assert that we're not accessing pixels outside the image */
	assert(x >= 0 && x < w && y >= 0 && y < h);

	/* Absolute bit position starts at position 0, the most-significant bit in the entire array */
	int bit = (y * h) + x;
	int byte = bit / 8;

	/* MSB = first */
	unsigned char offset = 7 - bit % 8;
	unsigned char bitmask = 1 << offset;

	return (image[byte] & bitmask) >> offset;
}