#include "game/state.h"
#include "game/level.h"
#include "game/entity.h"
#include "game/camera.h"

#include "hw/display.h"
#include "hw/input.h"

#include <assert.h>
#include <string.h>
#include <stdint.h>

/* Level we're playing right now */
Level current_level;
/* Pointer to player entity */
Entity* player_entity;

void level_load(Level data) {
	current_level = data;

	/* Reset and load entities */
	memset(entities, 0, (MAX_ENTITIES + 1) * sizeof(Entity));
	player_entity = NULL;
	entity_load_from_level_tiles(data, entities);

	/* Find the player */
	Entity* current_entity = entities;

	while(current_entity->type != ENTITY_TYPE_NONE) {
		if(current_entity->type == ENTITY_TYPE_PLAYER) {
			player_entity = current_entity;
			break;
		}

		current_entity++;
	}

	assert(player_entity != NULL);
	camera_reset(player_entity);
}

void level_update(int framenum) {
	entity_update_all(framenum);
	camera_update(framenum);
}

USE_IMAGE(tile_ground);
USE_IMAGE(tile_hazard);

/* Draw tiles at specified position */
void draw_tile(LevelTile tile, int x, int y) {
	switch(level_extract_tile_id(tile)) {
		case TILE_ID_GROUND: {
			/* Offset calculated by builder_image */
			uint8_t offset = level_extract_tile_data(tile);
			uint8_t offset_x = offset & 0b11;
			uint8_t offset_y = (offset & 0b1100) >> 2;

			/* Draw nothing if offset = 3 3 */
			if(offset_x != 3 || offset_y != 3) {
				display_draw_image_region(image_tile_ground, x, y, offset_x * TILE_SIZE, offset_y * TILE_SIZE, 8, 8, 0);
			}
			break;
		}

		case TILE_ID_HAZARD: {
			/* Offset calculated by builder_image */
			uint8_t offset = level_extract_tile_data(tile);
			uint8_t offset_x = offset & 0b11;
			uint8_t offset_y = (offset & 0b1100) >> 2;

			display_draw_image_region(image_tile_hazard, x, y, offset_x * TILE_SIZE, offset_y * TILE_SIZE, 8, 8, 0);
			break;
		}
	}
}

void level_draw() {
	/* Start drawing at top corner of camera */
	int tilestartx = camera_offset_x / TILE_SIZE;
	int tilestarty = camera_offset_y / TILE_SIZE;
	int tileendx = tilestartx + DISPLAY_WIDTH / TILE_SIZE;
	int tileendy = tilestarty + DISPLAY_HEIGHT / TILE_SIZE;

	/* Don't go out of bounds */
	if(tileendx >= level_w(current_level)) {
		tileendx = level_w(current_level) - 1;
	}
	if(tileendy >= level_h(current_level)) {
		tileendy = level_h(current_level) - 1;
	}

	for(int tiley = tilestarty; tiley != tileendy + 1; ++tiley) {
		for(int tilex = tilestartx; tilex != tileendx + 1; ++tilex) {
			LevelTile tile = current_level[(tiley * level_w(current_level)) + tilex];
			draw_tile(tile, tilex * TILE_SIZE - camera_offset_x, tiley * TILE_SIZE - camera_offset_y);
		}
	}

	entity_draw_all();
}