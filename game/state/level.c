#include "game/state.h"
#include "game/level.h"
#include "game/entity.h"
#include "game/camera.h"

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
	entity_update_all(current_level, entities);
	camera_update(framenum);
}

void level_draw() {
	entity_draw_all(current_level, entities);
}