#include "game/state.h"
#include "game/level.h"
#include "game/entity.h"
#include "game/camera.h"

#include "hw/display.h"

#include <assert.h>
#include <string.h>

/* Level we're playing right now */
Level current_level;
/* Spawned entities */
Entity entities[MAX_ENTITIES + 1]; /* last element will always have type none */
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
	entity_update_all(entities);
	camera_update(framenum);
}

USE_IMAGE(tile_ground);

void draw_tile(LevelTile tile, int x, int y) {
	switch(level_extract_tile_id(tile)) {
		case TILE_ID_GROUND:
			display_draw_image_region(image_tile_ground, x, y, 8, 0, 8, 8, 0); /* TODO offset */
			break;
	}
}

void level_draw() {
	/* Start drawing at top corner of camera */
	int tilestartx = camera_offset_x / TILE_SIZE;
	int tilestarty = camera_offset_y / TILE_SIZE;
	int tileendx = tilestartx + DISPLAY_WIDTH / TILE_SIZE;
	int tileendy = tilestarty + DISPLAY_HEIGHT / TILE_SIZE;

	/* Don't go out of bounds */
	if(tileendx >= level_width(current_level)) {
		tileendx = level_width(current_level) - 1;
	}
	if(tileendy >= level_height(current_level)) {
		tileendy = level_height(current_level) - 1;
	}

	for(int tiley = tilestarty; tiley != tileendy + 1; ++tiley) {
		for(int tilex = tilestartx; tilex != tileendx + 1; ++tilex) {
			LevelTile tile = current_level[(tiley * level_width(current_level)) + tilex];
			draw_tile(tile, tilex * TILE_SIZE - camera_offset_x, tiley * TILE_SIZE - camera_offset_y);
		}
	}

	entity_draw_all(entities);
}