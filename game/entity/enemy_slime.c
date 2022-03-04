#include "game/entity.h"
#include "game/camera.h"
#include "hw/display.h"

USE_IMAGE(enemy_slime);

/* 0.06 */
#define GRAVITY 6

void entity_slime_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata) {
	/* Spawn slime on top of tile */
	self->x = (tilex * TILE_SIZE) + TILE_SIZE / 2 - ENTITY_SLIME_WIDTH / 2;
	self->y = (tiley * TILE_SIZE) + TILE_SIZE - ENTITY_SLIME_HEIGHT;
}

void entity_slime_update(Entity* self, int framenum) {
	short speed_short = (short)(self->data & 0xffff);
	short speed_rshort = (short)((self->data & 0xffff0000) >> 16);

	int speed_x = 0;
	/* Widen variables */
	int speed_y = (int)speed_short;
	int speed_ry = (int)speed_rshort;

	/* Random jump timing */
	if((framenum + self->x * 12 + self->y * 15) % (60 * 3) == 0) {
		speed_y = -250;
	}

	/* Always apply constant gravity */
	speed_y += GRAVITY;

	entity_move(self, &speed_x, &speed_y, NULL, &speed_ry, true);

	/* Narrow variables */
	speed_short = (short)speed_y;
	speed_rshort = (short)speed_ry;

	self->data = speed_short | (speed_rshort << 16);
}

void entity_slime_draw(Entity* self) {
	bool on_ground = false;

	for(int x = self->x; x < self->x + ENTITY_SLIME_WIDTH; x++) {
		if(entity_try_collide_all(self, x, self->y + ENTITY_SLIME_HEIGHT)) {
			on_ground = true;
			break;
		}
	}

	display_draw_image_region(image_enemy_slime, self->x - camera_offset_x, self->y - camera_offset_y, 0, on_ground ? 0 : 10, ENTITY_SLIME_WIDTH, ENTITY_SLIME_HEIGHT, OP_OVERLAY);
}

bool entity_slime_try_collide(Entity* self, Entity* colliding_entity, int x, int y) {
	bool collide = x >= self->x && x < self->x + ENTITY_SLIME_WIDTH &&
		y >= self-> y && y < self->y + ENTITY_SLIME_HEIGHT;

	if(colliding_entity->type == ENTITY_TYPE_PLAYER && collide) {
		entity_kill(colliding_entity);
	}

	return collide;
}