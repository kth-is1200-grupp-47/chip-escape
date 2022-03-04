/* Written by Hannes Mann */

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "game/level.h"

#define STBI_ONLY_PNG
#include "stb/stb_image.h"

/* Temporary output file name */
#define TMP_OUTPUT_FORMAT "%s.data"
/* ld command format for sprintf */
#define LD_FORMAT "$LD -r -b binary -o %s %s"

uint32_t get_image_color_at(uint8_t* image, int x, int y, int iw, int ih) {
	/* Outside image */
	if(x < 0 || y < 0 || x >= iw || y >= ih) {
		return 0;
	}

	int input_pos = (y * iw + x) * 4;

	uint32_t color = image[input_pos + 3] ? 255 : 0 /* A */;
	color |= (image[input_pos + 2] << 8) /* B */;
	color |= (image[input_pos + 1] << 16) /* G */;
	color |= (image[input_pos] << 24) /* R */;

	return color;
}

void write_tile(uint8_t* input, uint8_t* output, uint32_t color, int x, int y, int iw, int ih) {
	/* Is pixel not transparent? */
	if(color & 255) {
		/* Match pixel color with tile id */
		switch(color) {
			/* Ground */
			case 0xffffffff: {
				*output = TILE_ID_GROUND;

				int offset_x = 1;
				int offset_y = 0;

				bool empty_left = get_image_color_at(input, x - 1, y, iw, ih) != color;
				bool empty_right = get_image_color_at(input, x + 1, y, iw, ih) != color;
				bool empty_top = get_image_color_at(input, x, y - 1, iw, ih) != color;
				bool empty_bottom = get_image_color_at(input, x, y + 1, iw, ih) != color;

				/* No ground tile to the left? */
				if(empty_left && x > 0) {
					offset_x = 0;
				}
				/* No ground tile to the right? */
				if(empty_right && x < iw - 1) {
					/* If previous statement is also true */
					if(offset_x == 0) {
						offset_x = 1;
						offset_y = 1;
					}
					else {
						offset_x = 2;
					}
				}

				/* Tile above us? */
				if(!empty_top && y > 0) {
					offset_y++;

					if(!empty_left && !empty_right) {
						offset_x = 3; /* draw nothing */
						offset_y = 3;
					}
					if(x == 0 || x == iw - 1) {
						offset_x = 3; /* draw nothing */
						offset_y = 3;
					}
				}

				/* Tile below us? */
				if(empty_bottom && y < ih - 1) {
					offset_x = 3;

					if(empty_left) {
						offset_y = 0;
					}
					else if(empty_right) {
						offset_y = 1;
					}
					else {
						offset_y = 2;
					}
				}

				/* TODO: slope */

				*output |= offset_x << 4;
				*output |= offset_y << 6;
				break;
			}

			/* Player */
			case 0xff0000ff:
				*output = TILE_ID_PLAYER;
				break;

			/* Platforms */
			case 0x00ff00ff:
			case 0x008000ff:
				bool falling = color == 0x008000ff;

				/* Platform already read - skip this pixel */
				if(get_image_color_at(input, x - 1, y, iw, ih) == color) {
					break;
				}

				/* Find neighbor pixels */
				if(get_image_color_at(input, x + 1, y, iw, ih) == color) {
					*output = TILE_ID_PLATFORM;

					/* Set bits for platform */
					if(falling) {
						*output |= TILE_PLATFORM_BIT_FALL << 4;
					}

					/* Is big platform? */
					if(get_image_color_at(input, x + 2, y, iw, ih) == color) {
						*output |= TILE_PLATFORM_BIT_BIG << 4;
					}
					else {
						*output |= TILE_PLATFORM_BIT_SMALL << 4;
					}
				}
				else {
					printf("builder_image: Platform too small at %d %d\n", x, y);
					exit(1);
				}

				break;

			/* Hazard */
			case 0x0000ffff:
			case 0x0080ffff: {
				*output = TILE_ID_HAZARD;

				int offset_x = color == 0x0080ffff;
				int offset_y = 0;

				*output |= offset_x << 4;
				*output |= offset_y << 6;
				break;
			}

			/* Bits */
			case 0x878700ff:
			case 0xafaf00ff:
			case 0xd7d700ff:
			case 0xffff00ff: {
				*output = TILE_ID_BITS;

				uint8_t bit_count = 1;
				if(color == 0xafaf00ff) { bit_count = 2; }
				if(color == 0xd7d700ff) { bit_count = 3; }
				if(color == 0xffff00ff) { bit_count = 4; }

				*output |= bit_count << 4;
				break;
			}

			/* Metal Block */
			case 0x808080ff:
				*output = TILE_ID_METAL_BLOCK;
				break;

			case 0xff00ffff:
				*output = TILE_ID_FLAG;
				/* This is the lower part of the flag */
				if(get_image_color_at(input, x, y - 1, iw, ih) == color) {
					*output |= 1 << 4;
				}
				break;

			default:
				printf("builder_image: Unknown tile type 0x%x at %d %d\n", color, x, y);
				exit(1);
		}
	}
}

/* Based on build_image_object */
void build_level_object(const char* input, const char* output) {
	int w, h, n;

	/* File being loaded */
	uint8_t* input_bytes = stbi_load(input, &w, &h, &n, 4);

	if(!input_bytes) {
		printf("builder_image: stb error %s\n", stbi_failure_reason());
		exit(1);
	}

	if(w < 128 / 8 || h < 32 / 8) {
		printf("builder_image: Level is too small! Width: %d, Height: %d\n", w, h);
		exit(1);
	}
	if(w > UINT16_MAX || h > UINT16_MAX) {
		printf("builder_image: Level is too big! Width: %d, Height: %d\n", w, h);
		exit(1);
	}

	/* File being written */
	int total_bits = sizeof(uint16_t) * 8 * 2 /* w+h */ + sizeof(LevelTile) * 8 * w * h /* x bits per tile */;
	int required_bytes = total_bits / 8 + (total_bits % 8 != 0);

	uint8_t* output_bytes = malloc(required_bytes);
	/* Set all tiles to TILE_ID_NONE by default */
	memset(output_bytes, 0, required_bytes);

	/* Set first four bytes to size */
	uint16_t* output_as_short = (uint16_t*)output_bytes;
	output_as_short[0] = w;
	output_as_short[1] = h;

	int current_byte = 4;

	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			int input_pos = (y * w + x) * 4;
			/* Assert that we're not reading past the end of the input image */
			assert(input_pos + 3 < w * h * 4 /* four bits per pixel */);
			/* Assert that we're not writing past the end of the output data */
			assert(current_byte < required_bytes);

			uint32_t color = get_image_color_at(input_bytes, x, y, w, h);
			write_tile(input_bytes, output_bytes + current_byte++, color, x, y, w, h);
		}
	}

	char* tmp_output_name = malloc(strlen(TMP_OUTPUT_FORMAT) + strlen(input) + 1);
	sprintf(tmp_output_name, TMP_OUTPUT_FORMAT, input);

	/* Create a temporary output file in /tmp to use as argument for ld */
	FILE* tmp_output_file = fopen(tmp_output_name, "wb");
	fwrite(output_bytes, sizeof(uint8_t), required_bytes, tmp_output_file);
	fclose(tmp_output_file);

	/* Create the ld command to embed the data in an object file that will be linked to the final binary */
	char* command = malloc(strlen(LD_FORMAT) + strlen(tmp_output_name) + strlen(output) + 1);
	sprintf(command, LD_FORMAT, output, tmp_output_name);

	/* Execute command and delete temporary output file */
	system(command);
	unlink(tmp_output_name);

	free(tmp_output_name);
	free(command);

	free(output_bytes);
	stbi_image_free(input_bytes);
}