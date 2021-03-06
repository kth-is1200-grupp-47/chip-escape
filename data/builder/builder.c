/*
 * This program runs on the local machine and transforms assets into arrays that are easy to read from C.
 *
 * Image format:
 * Byte 0-1 - Width, Height
 * Byte 2-x - Eight bits containing up to eight pixels (order: LSB -> MSB)
 *
 * Bits are arranged as y rows with x columns in them. To locate a bit: (y * width + x) / 8
 *
 * Level format:
 * Byte 0-1 - Width
 * Byte 2-3 - Height
 * Byte 4-x - Eight bits containing a level tile (upper bits: data, lower bits: ID)
 *
 * Bits are arranged as y rows with x columns in them. To locate a bit: (y * width + x) / 8
 *
 * Written by Hannes Mann
 */

#include <stdio.h>
#include <string.h>

/* Build stb in this file */
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb/stb_image.h"

void build_image_object(const char* input, const char* output);
void build_level_object(const char* input, const char* output);

int main(int argc, char** argv) {
	if(!strcmp("-image", argv[1])) {
		build_image_object(argv[2], argv[3]);
	}
	if(!strcmp("-level", argv[1])) {
		build_level_object(argv[2], argv[3]);
	}

	printf("%s: Done building object file \"%s\".\n", argv[2], argv[3]);
	return 0;
}