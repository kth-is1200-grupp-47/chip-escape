/*
 * Macros for accessing embedded data.
 * USE_X(name) macros are used for defining the external variable at the top of the file.
 *
 * Written by Hannes Mann
 */

#include <stdbool.h>

#ifndef MACROS_H
#define MACROS_H

#define USE_IMAGE(name) extern unsigned char _binary_data_images_ ## name ## _png_start[]

#define IMAGE_WIDTH(name) (_binary_data_images_ ## name ## _png_start[0])
#define IMAGE_HEIGHT(name) (_binary_data_images_ ## name ## _png_start[1])
#define IMAGE_DATA(name) (&_binary_data_images_ ## name ## _png_start[2])

/* Access a pixel of an image. The image pointer should be retrieved with IMAGE_DATA(name). */
bool image_bit(unsigned char* image, int x, int y);

#endif