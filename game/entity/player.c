#include "game/camera.h"
#include "game/entity.h"

#include "hw/display.h"

USE_IMAGE(player);

void entity_player_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata) {
	/* Spawn player at center of tile */
	self->x = (tilex * TILE_SIZE) + TILE_SIZE / 2 - width_of(image_player) / 2;
	self->y = (tiley * TILE_SIZE) + TILE_SIZE / 2 - height_of(image_player) / 2;
}

void entity_player_update(Entity* self) {

}

void entity_player_draw(Entity* self) {
	display_draw_image(image_player, self->x - camera_offset_x, self->y - camera_offset_y, 0);
}