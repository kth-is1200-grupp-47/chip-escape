/*
 * Structures and definitions for level data. Included in both builder and game.
 * Written by Hannes Mann
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>

typedef uint8_t LevelTile;

/* Width/height of tile */
#define TILE_SIZE 8

/* 4-bit integer indicating the type of tile in part of a level. */
#define TILE_ID_NONE 0
/* Upper bits: Offset in tile_ground.png */
#define TILE_ID_GROUND 1
/* Upper bits: Size */
#define TILE_ID_PLATFORM 2
/* Indicates that player should spawn at this position */
#define TILE_ID_PLAYER 15

/* Bit indicating size of platform */
#define TILE_PLATFORM_BIT_SMALL 1
#define TILE_PLATFORM_BIT_BIG 2
/* Bit indicating if platform falls when player steps on it */
#define TILE_PLATFORM_BIT_FALL 4

/* Extract lower 4 bits from level tile (ID) */
static inline uint8_t level_extract_tile_id(LevelTile tile) {
	return tile & 0xf;
}
/* Extract upper 4 bits from level tile (data) */
static inline uint8_t level_extract_tile_data(LevelTile tile) {
	return tile >> 4;
}

#endif