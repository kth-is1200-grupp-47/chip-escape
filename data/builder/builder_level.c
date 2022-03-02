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
			case 0xffffffff:
				*output = TILE_ID_GROUND;
				break;

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
						*output |= TILE_PLATFORM_BIT_FALL;
					}

					/* Is big platform? */
					if(get_image_color_at(input, x + 2, y, iw, ih) == color) {
						*output |= TILE_PLATFORM_BIT_BIG;
					}
					else {
						*output |= TILE_PLATFORM_BIT_SMALL;
					}
				}
				else {
					printf("builder_image: Platform too small at %d %d\n", x, y);
					exit(1);
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