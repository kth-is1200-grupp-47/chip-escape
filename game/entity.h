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

/* Entity is dead (skip) */
#define ENTITY_KILLED 255

#define ENTITY_PLAYER_WIDTH 8
#define ENTITY_PLAYER_HEIGHT 11

typedef struct {
	/* Entity type */
	EntityType type;
	/* Position */
	int x, y;
	/* This is either a pointer to entity data, or a 32-bit integer containing data */
	uint32_t data;
} Entity;

extern Entity entities[MAX_ENTITIES + 1];

/* Spawns entities from some level tiles */
void entity_load_from_level_tiles(Level level, Entity* array_start);

/* Spawn a new ENTITY_TYPE_PLAYER */
void entity_player_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata);
/* Spawn a new ENTITY_TYPE_PLATFORM */
void entity_platform_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata);

/* Update all entities in array */
void entity_update_all(int framenum);
/* Draw all entities in array */
void entity_draw_all();

/* Returns true if any entity or tile collided at the specified position */
bool entity_try_collide_all(int x, int y);

/* Update ENTITY_TYPE_PLAYER */
void entity_player_update(Entity* self, int framenum);
/* Draw ENTITY_TYPE_PLAYER */
void entity_player_draw(Entity* self);

/* Update ENTITY_TYPE_PLATFORM */
void entity_platform_update(Entity* self, int framenum);
/* Draw ENTITY_TYPE_PLATFORM */
void entity_platform_draw(Entity* self);
/* Try collide ENTITY_TYPE_PLATFORM */
bool entity_platform_try_collide(Entity* self, int x, int y);

/* Moves entity mx/my pixels. mx/my are divided by 100 to allow for more precision. */
void entity_move(Entity* entity, int* mx, int* my, int* rx, int* ry, bool collision);

/* Kill entity by removing from list (and running a function if it exists) */
void entity_kill(Entity* entity);
#endif