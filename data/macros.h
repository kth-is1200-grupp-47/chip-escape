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
	Image image_##name = _binary_data_images_ ##name## _png_data_start + 2;

static inline uint8_t width_of(Image image) { return image[-2]; }
static inline uint8_t height_of(Image image) { return image[-1]; }

/* Access a single pixel of an image. */
static bool image_bit(Image image, int x, int y) {
	/* Wrap around if coordinates are outside the image. */
	x = x % width_of(image);
	y = y % height_of(image);

	int bit = (y * width_of(image)) + x;
	/* LSB = first */
	uint8_t bitmask = 1 << (bit % 8);

	return image[bit >> 3 /* bitwise for speed */] & bitmask;
}

/* Pointer to level data from builder.c */
typedef const uint8_t* Level;

#define USE_LEVEL(name) \
	extern const uint8_t _binary_data_levels_ ##name## _png_data_start[]; \
	Level level_##name = _binary_data_levels_ ##name## _png_data_start + sizeof(uint16_t) * 2;

static inline uint16_t level_width(Level level) { return (level[-4] << 8) | level[-3]; }
static inline uint16_t level_height(Level level) { return (level[-2] << 8) | level[-1]; }

#endif