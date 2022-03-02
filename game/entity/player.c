#include "game/entity.h"

USE_IMAGE(player);

void entity_player_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata) {
	/* Spawn player at center of tile */
	self->x = (tilex * TILE_SIZE) + TILE_SIZE / 2 - width_of(image_player) / 2;
	self->y = (tiley * TILE_SIZE) + TILE_SIZE / 2 - height_of(image_player) / 2;
}