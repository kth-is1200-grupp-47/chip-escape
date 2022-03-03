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

#define try_collide_entity(etype, function) \
	case etype: \
		function(current_entity, x, y); \
		break

bool entity_try_collide_all(Entity* array_start, int x, int y) {
	Entity* current_entity = array_start;

	while(current_entity->type != ENTITY_TYPE_NONE) {
		switch(current_entity->type) {
			try_collide_entity(ENTITY_TYPE_PLATFORM, entity_platform_try_collide);
		}

		current_entity++;
	}
}

void entity_move(Entity* entity, int mx, int my, int* rx, int* ry) {
	/* Integer number */
	entity->x += mx / 100;
	entity->y += my / 100;

	int absx = mx < 0 ? -mx : mx;
	int absy = my < 0 ? -my : my;

	/* Fractions */
	if(mx == 0) {
		*rx = 0;
	}
	else {
		/* Decimals 00-99 */
		*rx += absx % 100;

		int stepx = mx/absx;
		while(*rx >= 100) {
			entity->x += stepx;
			*rx -= 100;
		}
	}

	if(my == 0) {
		*ry = 0;
	}
	else {
		/* Decimals 00-99 */
		*ry += absy % 100;

		int stepy = my/absy;
		while(*ry >= 100) {
			entity->y += stepy;
			*ry -= 100;
		}
	}
}