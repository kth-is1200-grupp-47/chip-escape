#include "game/camera.h"
#include "game/entity.h"

#include "hw/display.h"
#include "hw/inputs.h"

/* From level.c */
extern Level current_level;

USE_IMAGE(player);

void entity_player_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata) {
	/* Spawn player at center of tile */
	self->x = (tilex * TILE_SIZE) + TILE_SIZE / 2 - width_of(image_player) / 2;
	self->y = (tiley * TILE_SIZE) + TILE_SIZE / 2 - height_of(image_player) / 2;
}

void entity_player_update(Entity* self) {
	int btns = getBtns();

	if(btns & (1 << 0)) {
		self->x++;
	}
	if(btns & (1 << 1)) {
		self->x--;
	}

	if(btns & (1 << 2)) {
		self->y++;
	}
	if(btns & (1 << 3)) {
		self->y--;
	}
}

void entity_player_draw(Entity* self) {
	display_draw_image(image_player, self->x - camera_offset_x, self->y - camera_offset_y, 0);
}