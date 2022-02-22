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
	extern const uint8_t _binary_data_images_ ##name## _png_start[]; \
	Image image_##name = _binary_data_images_ ##name## _png_start + 2;

static inline uint8_t width_of(Image image) { return image[-2]; }
static inline uint8_t height_of(Image image) { return image[-1]; }

/* Access a single pixel of an image. */
bool image_bit(Image image, int x, int y);

#endif