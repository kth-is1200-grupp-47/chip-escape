#include "game/entity.h"
#include "debug.h"
#include <assert.h>

/* From level.c */
extern Level current_level;
extern Entity entities[MAX_ENTITIES + 1];

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
		if(function(current_entity, x, y)) { return true; } \
		break

bool entity_try_collide_all(Entity* array_start, int x, int y) {
	Entity* current_entity = array_start;

	while(current_entity->type != ENTITY_TYPE_NONE) {
		switch(current_entity->type) {
			try_collide_entity(ENTITY_TYPE_PLATFORM, entity_platform_try_collide);
		}

		current_entity++;
	}

	uint8_t tile_at_xy = current_level[(y / TILE_SIZE) * level_width(current_level) + x / TILE_SIZE];
	uint8_t tile_id = level_extract_tile_id(tile_at_xy);

	for(int i = 0; i < sizeof(solid_tiles); i++) {
		if(tile_id == solid_tiles[i]) {
			return true;
		}
	}

	return false;
}

void entity_size(Entity* entity, int* x, int* y) {
	switch(entity->type) {
		case ENTITY_TYPE_PLAYER:
			*x = ENTITY_PLAYER_WIDTH;
			*y = ENTITY_PLAYER_HEIGHT;
			break;
	}
}

/* Returns false if entity was stopped */
bool entity_step_x(Entity* entity, int dir, bool collision) {
	if(collision) {
		int sizex = 0;
		int sizey = 0;
		entity_size(entity, &sizex, &sizey);
		assert(sizex != 0 && sizey != 0);

		int collidex = dir == -1 ? entity->x - 1 : entity->x + sizex;
		for(int y = entity->y; y < entity->y + sizey; y++) {
			if(entity_try_collide_all(entities, collidex, y)) {
				return false;
			}

		}
	}

	entity->x += dir;
	return true;
}

/* Returns false if entity was stopped */
bool entity_step_y(Entity* entity, int dir, bool collision) {
	if(collision) {
		int sizex = 0;
		int sizey = 0;
		entity_size(entity, &sizex, &sizey);
		assert(sizex != 0 && sizey != 0);

		int collidey = dir == -1 ? entity->y - 1 : entity->y + sizey;
		for(int x = entity->x; x < entity->x + sizex; x++) {
			if(entity_try_collide_all(entities, x, collidey)) {
				return false;
			}
		}
	}

	entity->y += dir;
	return true;
}

void entity_move(Entity* entity, int* mx, int* my, int* rx, int* ry, bool collision) {
	int absx = *mx < 0 ? -*mx : *mx;
	int absy = *my < 0 ? -*my : *my;

	/* Integer number */
	for(int x = 0; x < absx / 100; x++) {
		if(!entity_step_x(entity, *mx < 0 ? -1 : 1, collision)) {
			*mx = 0;
			break;
		}
	}
	for(int y = 0; y < absy / 100; y++) {
		if(!entity_step_y(entity, *my < 0 ? -1 : 1, collision)) {
			*my = 0;
			break;
		}
	}

	/* Fractions */
	if(rx) {
		if(*mx == 0) {
			*rx = 0;
		}
		else {
			/* Decimals 00-99 */
			*rx += absx % 100;

			int stepx = *mx/absx;
			while(*rx >= 100) {
				if(!entity_step_x(entity, *mx < 0 ? -1 : 1, collision)) {
					*mx = 0;
					*rx = 0;
					break;
				}

				*rx -= 100;
			}
		}
	}

	if(ry) {
		if(my == 0) {
			*ry = 0;
		}
		else {
			/* Decimals 00-99 */
			*ry += absy % 100;

			int stepy = *my/absy;
			while(*ry >= 100) {
				if(!entity_step_y(entity, *my < 0 ? -1 : 1, collision)) {
					*my = 0;
					*ry = 0;
					break;
				}

				*ry -= 100;
			}
		}
	}
}