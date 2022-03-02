#include "game/entity.h"
#include "debug.h"
#include <assert.h>

#define check_spawn_entity(etile, etype, function) \
	case etile: \
		current_entity->type = etype; \
		function(current_entity, x, y, tile); \
		current_entity++; \
		break

void entity_load_from_level_tiles(Level level, Entity* array_start) {
	Entity* current_entity = array_start;

	for(int y = 0; y != level_height(level); y++) {
		for(int x = 0; x != level_width(level); x++) {
			LevelTile tile = level[(y * level_width(level)) + x];
			uint8_t type = level_extract_tile_id(tile);

			switch(type) {
				check_spawn_entity(TILE_ID_PLAYER, ENTITY_TYPE_PLAYER, entity_player_spawn);
				check_spawn_entity(TILE_ID_PLATFORM, ENTITY_TYPE_PLATFORM, entity_platform_spawn);
			}

			/* Ensure we don't spawn too many entities */
			assert(current_entity < array_start + MAX_ENTITIES);
		}
	}
}

#define update_entity(etype, function) \
	case etype: \
		function(current_entity); \
		break

void entity_update_all(Entity* array_start) {
	Entity* current_entity = array_start;

	while(current_entity->type != ENTITY_TYPE_NONE) {
		switch(current_entity->type) {
			update_entity(ENTITY_TYPE_PLAYER, entity_player_update);
			update_entity(ENTITY_TYPE_PLATFORM, entity_platform_update);
		}

		current_entity++;
	}
}

void entity_draw_all(Entity* array_start) {
	Entity* current_entity = array_start;

	while(current_entity->type != ENTITY_TYPE_NONE) {
		switch(current_entity->type) {
			update_entity(ENTITY_TYPE_PLAYER, entity_player_draw);
			update_entity(ENTITY_TYPE_PLATFORM, entity_platform_draw);
		}

		current_entity++;
	}
}