/* Written by Hannes Mann */

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

/* Temporary output file name */
#define TMP_OUTPUT_FORMAT "%s.data"
/* ld command format for sprintf */
#define LD_FORMAT "$LD -r -b binary -o %s %s"

void build_image_object(const char* input, const char* output) {
	int w, h, n;

	/* File being loaded */
	uint8_t* input_bytes = stbi_load(input, &w, &h, &n, 4);

	if(!input_bytes) {
		printf("%s: stb error %s\n", stbi_failure_reason());
		exit(1);
	}

	/* TODO: Bigger images needed in the game? */
	if(w > 255 || h > 255) {
		printf("%s: Image is too big! Width: %d, Height: %d\n", input, w, h);
		exit(1);
	}

	/* File being written */
	int total_bits = 8 * 2 /* w+h */ + w * h /* one bit per pixel */;
	int required_bytes = total_bits / 8 + (total_bits % 8 != 0);

	uint8_t* output_bytes = malloc(required_bytes);
	/* Set all pixels to black by default */
	memset(output_bytes, 0, required_bytes);

	output_bytes[0] = w;
	output_bytes[1] = h;

	int current_byte = 2;
	int current_bit = 0;

	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			int input_pos = (y * w + x) * 4;
			/* Assert that we're not reading past the end of the input image */
			assert(input_pos + 3 < w * h * 4 /* four bits per pixel */);
			/* Assert that we're not writing past the end of the output data */
			assert(current_byte < required_bytes);
			assert(current_bit >= 0 && current_bit <= 7);

			uint8_t red = input_bytes[input_pos + 0];
			uint8_t green = input_bytes[input_pos + 1];
			uint8_t blue = input_bytes[input_pos + 2];
			/* TODO: Alpha needed in the game? */
			uint8_t alpha = input_bytes[input_pos + 3];

			/* We only write to the array if a pixel is white because the array is zeroed */
			if(red == 255 && green == 255 && blue == 255) {
				output_bytes[current_byte] |= 1 << current_bit;
			}
			else if(red || green || blue) {
				printf("%s: Only white and black pixels are allowed in PNG files.\n", input);
				exit(1);
			}

			if(++current_bit > 7) {
				current_byte++;
				current_bit = 0;
			}
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