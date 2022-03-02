/*
 * Entites are "things" that can move around in the game world.
 * Written by Hannes Mann
 */

#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>

#include "data/macros.h"
#include "game/level.h"

typedef uint8_t EntityType;

/* Max entities in world */
#define MAX_ENTITIES 64

/* Not an entity */
#define ENTITY_TYPE_NONE 0
/* The player (only one of these can exist) */
#define ENTITY_TYPE_PLAYER 1

/* Floating platform */
#define ENTITY_TYPE_PLATFORM 100

typedef struct {
	/* Entity type */
	EntityType type;
	/* Position */
	int x, y;
	/* This is either a pointer to entity data, or a 32-bit integer containing data */
	void* data; /* TODO: safe? */
} Entity;

/* Spawns entities from some level tiles */
void entity_load_from_level_tiles(Level level, Entity* array_start);

/* Spawn a new ENTITY_TYPE_PLAYER */
void entity_player_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata);
/* Spawn a new ENTITY_TYPE_PLATFORM */
void entity_platform_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata);

/* Update all entities in array */
void entity_update_all(Entity* array_start);
/* Draw all entities in array */
void entity_draw_all(Entity* array_start);

/* Update ENTITY_TYPE_PLAYER */
void entity_player_update(Entity* self);
/* Draw ENTITY_TYPE_PLAYER */
void entity_player_draw(Entity* self);

/* Update ENTITY_TYPE_PLATFORM */
void entity_platform_update(Entity* self);
/* Draw ENTITY_TYPE_PLATFORM */
void entity_platform_draw(Entity* self);

#endif