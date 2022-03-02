#include "game/camera.h"
#include "hw/display.h"

USE_IMAGE(player);

/* From level.c */
extern Level current_level;

/* Player the camera is following */
Entity* following_player;

/* Offset calculated in camera_update */
int camera_offset_x;
int camera_offset_y;

void camera_reset(Entity* player) {
	following_player = player;
}

void camera_update(int framenum) {
	camera_offset_x = following_player->x + width_of(image_player) / 2 - DISPLAY_WIDTH / 2;
	camera_offset_y = following_player->y + height_of(image_player) / 2 - DISPLAY_HEIGHT / 2;

	if(camera_offset_x < 0) {
		camera_offset_x = 0;
	}
	if(camera_offset_x > level_width(current_level) * TILE_SIZE - DISPLAY_WIDTH - 1) {
		camera_offset_x = level_width(current_level) * TILE_SIZE - DISPLAY_WIDTH - 1;
	}

	if(camera_offset_y < 0) {
		camera_offset_y = 0;
	}
	if(camera_offset_y > level_height(current_level) * TILE_SIZE - DISPLAY_HEIGHT - 1) {
		camera_offset_y = level_height(current_level) * TILE_SIZE - DISPLAY_HEIGHT - 1;
	}
}