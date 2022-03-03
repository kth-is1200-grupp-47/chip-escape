/*
 * Macros for accessing embedded data.
 * USE_X(name) macros are used for defining the external variable at the top of the file.
 *
 * Written by Hannes Mann
 */

#include <stdbool.h>
#include <stdint.h>

#ifndef MACROS_H
#define MACROS_H

/* Pointer to image data from builder.c */
typedef const uint8_t* Image;

#define USE_IMAGE(name) \
	extern const uint8_t _binary_data_images_ ##name## _png_data_start[]; \
	static Image image_##name = _binary_data_images_ ##name## _png_data_start + 2;

static inline uint8_t image_w(Image image) { return image[-2]; }
static inline uint8_t image_h(Image image) { return image[-1]; }

/* Access a single pixel of an image. */
static bool image_bit(Image image, int x, int y) {
	/* Wrap around if coordinates are outside the image. */
	x = x % image_w(image);
	y = y % image_h(image);

	int bit = (y * image_w(image)) + x;
	/* LSB = first */
	uint8_t bitmask = 1 << (bit % 8);

	return image[bit >> 3 /* bitwise for speed */] & bitmask;
}

/* Pointer to level data from builder.c */
typedef const uint8_t* Level;

#define USE_LEVEL(name) \
	extern const uint8_t _binary_data_levels_ ##name## _png_data_start[]; \
	static Level level_##name = _binary_data_levels_ ##name## _png_data_start + sizeof(uint16_t) * 2;

static inline uint16_t level_w(Level level) { return level[-4] | (level[-3] << 8); }
static inline uint16_t level_h(Level level) { return level[-2] | (level[-1] << 8); }

#endif