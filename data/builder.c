/*
 * This program runs on the local machine and transforms assets into arrays that are easy to read from C.
 *
 * Image format:
 * Byte 0-1 - Width, Height
 * Byte 2-x - Eight bits containing up to eight pixels (order: MSB -> LSB)
 *
 * TODO: Change image format if necessary to match better with the ChipKIT
 *
 * Written by Hannes Mann
 */

#include <stdio.h>
#include <string.h>

void build_image_object(const char* input, const char* output);

int main(int argc, char** argv) {
	/* Image loader */
	if(!strcmp("-image", argv[1])) {
		build_image_object(argv[2], argv[3]);
	}

	printf("%s: Done building object file \"%s\".\n", argv[2], argv[3]);
	return 0;
}