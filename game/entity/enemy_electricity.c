#include "game/entity.h"
#include "game/camera.h"
#include "hw/display.h"

USE_IMAGE(enemy_electricity);

int electricity_animation_frame;

void entity_electricity_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata) {
	self->x = tilex * TILE_SIZE;
	self->y = tiley * TILE_SIZE;

	/* MSB: Direction */
	self->data = 16 * 4;
}

void entity_electricity_update(Entity* self, int framenum) {
	electricity_animation_frame = framenum;

	/* Pixels left until we turn around */
	int pixels_left = self->data & 0x7FFFFFFF;
	bool go_left = self->data & 0x80000000;

	if(go_left) {
		/* If we would fall when moving left change direction */
		if(!entity_try_collide_all(self, self->x - 1, self->y + 9) ||
			entity_try_collide_all(self, self->x - 1, self->y + 4)) {
			self->data = 16 * 4;
			self->data &= ~0x80000000;
		}
		else if(framenum % 2 == 0) {
			self->x--;
			pixels_left--;
			self->data &= 0x80000000;
			self->data |= pixels_left;
		}
	}
	else {
		/* If we would fall when moving right change direction */
		if(!entity_try_collide_all(self, self->x + 8, self->y + 9)  ||
			entity_try_collide_all(self, self->x + 8, self->y + 4)) {
			self->data = 16 * 4;
			self->data |= 0x80000000;
		}
		else if(framenum % 2 == 0)  {
			self->x++;
			pixels_left--;
			self->data &= 0x80000000;
			self->data |= pixels_left;
		}
	}
}

void entity_electricity_draw(Entity* self) {
	int frame = (electricity_animation_frame / 10) * 8;
	frame = frame % 24;

	display_draw_image_region(image_enemy_electricity, self->x - camera_offset_x, self->y - camera_offset_y, frame, 0, 8, 8, OP_OVERLAY);
}

bool entity_electricity_try_collide(Entity* self, Entity* colliding_entity, int x, int y) {
	bool collide = x >= self->x && x < self->x + 8 &&
		y >= self-> y && y < self->y + 8;

	if(colliding_entity->type == ENTITY_TYPE_PLAYER && collide) {
		entity_kill(colliding_entity);
	}

	return collide;
}