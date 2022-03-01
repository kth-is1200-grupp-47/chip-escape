#include "game/state.h"
#include "game/level.h"
#include "game/entity.h"

#include <assert.h>
#include <string.h>

/* Level we're playing right now */
Level current_level;
/* Spawned entities */
Entity entities[MAX_ENTITIES + 1] /* last element will always have type none */;
/* Pointer to player entity */
Entity* player_entity = NULL;

void level_load(Level data) {
	current_level = data;

	/* Reset and load entities */
	memset(entities, 0, MAX_ENTITIES * sizeof(Entity));
	entity_load_from_level_tiles(data, entities);
}

void level_update(int framenum) {
	entity_update_all(current_level, entities);
}

void level_draw() {
	entity_draw_all(current_level, entities);
}