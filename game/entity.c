#include "game/entity.h"
#include "debug.h"
#include <assert.h>
#include <string.h>

/* From level.c */
extern Level current_level;

Entity entities[MAX_ENTITIES + 1];

#define spawn_entity(etile, etype, function) \
	case etile: \
		current_entity->type = etype; \
		function(current_entity, x, y, tile); \
		current_entity++; \
		break

void entity_load_from_level_tiles(Level level, Entity* array_start) {
	Entity* current_entity = array_start;

	for(int y = 0; y != level_h(level); y++) {
		for(int x = 0; x != level_w(level); x++) {
			LevelTile tile = level[(y * level_w(level)) + x];
			uint8_t type = level_extract_tile_id(tile);

			switch(type) {
				spawn_entity(TILE_ID_PLAYER, ENTITY_TYPE_PLAYER, entity_player_spawn);
				spawn_entity(TILE_ID_PLATFORM, ENTITY_TYPE_PLATFORM, entity_platform_spawn);
			}

			/* Ensure we don't spawn too many entities */
			assert(current_entity < array_start + MAX_ENTITIES);
		}
	}
}

#define update_entity(etype, function) \
	case etype: \
		function(current_entity, framenum); \
		break

void entity_update_all(int framenum) {
	Entity* current_entity = entities;

	while(current_entity->type != ENTITY_TYPE_NONE) {
		switch(current_entity->type) {
			update_entity(ENTITY_TYPE_PLAYER, entity_player_update);
			update_entity(ENTITY_TYPE_PLATFORM, entity_platform_update);
		}

		current_entity++;
	}
}

#define draw_entity(etype, function) \
	case etype: \
		function(current_entity); \
		break

void entity_draw_all() {
	Entity* current_entity = entities;

	while(current_entity->type != ENTITY_TYPE_NONE) {
		switch(current_entity->type) {
			draw_entity(ENTITY_TYPE_PLAYER, entity_player_draw);
			draw_entity(ENTITY_TYPE_PLATFORM, entity_platform_draw);
		}

		current_entity++;
	}
}

#define try_collide_entity(etype, function) \
	case etype: \
		if(function(current_entity, colliding_entity, x, y)) { return true; } \
		break

bool entity_try_collide_all(Entity* colliding_entity, int x, int y) {
	Entity* current_entity = entities;

	while(current_entity->type != ENTITY_TYPE_NONE) {
		switch(current_entity->type) {
			try_collide_entity(ENTITY_TYPE_PLATFORM, entity_platform_try_collide);
		}

		current_entity++;
	}

	uint8_t tile_at_xy = current_level[(y / TILE_SIZE) * level_w(current_level) + x / TILE_SIZE];
	uint8_t tile_id = level_extract_tile_id(tile_at_xy);

	for(int i = 0; i < sizeof(solid_tiles); i++) {
		if(tile_id == solid_tiles[i]) {
			return true;
		}
	}

	for(int i = 0; i < sizeof(dangerous_tiles); i++) {
		if(tile_id == dangerous_tiles[i]) {
			entity_kill(colliding_entity);
			return true;
		}
	}

	return false;
}

int entity_size_x(Entity* entity) {
	switch(entity->type) {
		case ENTITY_TYPE_PLAYER:
			return ENTITY_PLAYER_WIDTH;
	}

	return 0;
}

int entity_size_y(Entity* entity) {
	switch(entity->type) {
		case ENTITY_TYPE_PLAYER:
			return ENTITY_PLAYER_HEIGHT;
	}

	return 0;
}

/* Returns false if entity was stopped */
bool entity_step_x(Entity* entity, int dir, bool collision) {
	if(collision) {
		assert(entity_size_x(entity) != 0 && entity_size_y(entity) != 0);

		/* Check all pixels to left or right of entity */
		int collidex = dir == -1 ? entity->x - 1 : entity->x + entity_size_x(entity);
		for(int y = entity->y; y != entity->y + entity_size_y(entity); ++y) {
			if(entity_try_collide_all(entity, collidex, y)) {
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
		assert(entity_size_x(entity) != 0 && entity_size_y(entity) != 0);

		/* Check all pixels above or below entity */
		int collidey = dir == -1 ? entity->y - 1 : entity->y + entity_size_y(entity);
		for(int x = entity->x; x != entity->x + entity_size_x(entity); ++x) {
			if(entity_try_collide_all(entity, x, collidey)) {
				return false;
			}
		}
	}

	entity->y += dir;
	return true;
}

void entity_keep_in_world(Entity* entity) {
	/* Entites can't move outside the world in X axis */
	if(entity->x < 0) {
		entity->x = 0;
	}
	if(entity->x > level_w(current_level) * TILE_SIZE - entity_size_x(entity)) {
		entity->x = level_w(current_level) * TILE_SIZE - entity_size_x(entity);
	}
}

void entity_move(Entity* entity, int* mx, int* my, int* rx, int* ry, bool collision) {
	/* Absolute value of mx and my */
	int absx = *mx < 0 ? -*mx : *mx;
	int absy = *my < 0 ? -*my : *my;

	/* Attempt movement pixel by pixel for both X and Y axis */
	for(int x = 0; x != absx / 100; ++x) {
		if(!entity_step_x(entity, *mx < 0 ? -1 : 1, collision)) {
			*mx = 0;
			break;
		}
	}

	entity_keep_in_world(entity);

	for(int y = 0; y != absy / 100; ++y) {
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
			/* Extract remainder and add to total */
			*rx += absx % 100;

			while(*rx >= 100) {
				if(!entity_step_x(entity, *mx < 0 ? -1 : 1, collision)) {
					*mx = 0;
					*rx = 0;
					break;
				}

				*rx -= 100;
			}

			entity_keep_in_world(entity);
		}
	}

	if(ry) {
		if(my == 0) {
			*ry = 0;
		}
		else {
			/* Extract remainder and add to total */
			*ry += absy % 100;

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

#define kill_entity(etype, function) \
	case etype: \
		function(entity); \
		break


void entity_kill(Entity* entity) {
	/* Set type before running function so entity can revive */
	EntityType old_type = entity->type;
	entity->type = ENTITY_KILLED;

	switch(old_type) {
		kill_entity(ENTITY_TYPE_PLAYER, entity_player_kill);
	}
}