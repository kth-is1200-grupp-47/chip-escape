#include "game/level.h"
#include "game/entity.h"
#include "game/camera.h"

#include "hw/display.h"

#define PLATFORM_SMALL_WIDTH 13
#define PLATFORM_BIG_WIDTH 23
#define PLATFORM_HEIGHT 4

USE_IMAGE(tile_platform);

/* From level.c */
extern Level current_level;

uint8_t get_fall_frames(Entity* self) {
	return self->data >> 24;
}

void set_fall_frames(Entity* self, uint8_t f) {
	self->data &= 0x00ffffff;
	self->data |= f << 24;
}

void entity_platform_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata) {
	uint8_t bits = level_extract_tile_data(tiledata);
	/* Reinterpret data pointer as integer */
	self->data = bits;

	/* Spawn platform in center of tile */
	self->x = (tilex * TILE_SIZE) + TILE_SIZE /* center of two tiles */ - PLATFORM_SMALL_WIDTH / 2;
	self->y = (tiley * TILE_SIZE) + TILE_SIZE / 2 - PLATFORM_HEIGHT / 2;

	if(bits & TILE_PLATFORM_BIT_BIG) {
		self->x = (tilex * TILE_SIZE) + (TILE_SIZE * 3) / 2 /* center of three tiles */ - PLATFORM_BIG_WIDTH / 2;
	}
}

void entity_platform_update(Entity* self, int framenum) {
	/* Last eight bits of data are how many frames are left until platform falls */
	if(get_fall_frames(self) > 1) {
		set_fall_frames(self, get_fall_frames(self) - 1);
	}

	if(get_fall_frames(self) == 1) {
		self->y++;

		if(self->y > level_h(current_level) * TILE_SIZE + 10) {
			entity_kill(self);
		}
	}
}

void entity_platform_draw(Entity* self) {
	if(self->data & TILE_PLATFORM_BIT_BIG) {
		display_draw_image_region(image_tile_platform, self->x - camera_offset_x, self->y - camera_offset_y, 0, PLATFORM_HEIGHT, PLATFORM_BIG_WIDTH, PLATFORM_HEIGHT, 0);
	}
	else {
		display_draw_image_region(image_tile_platform, self->x - camera_offset_x, self->y - camera_offset_y, 0, 0, PLATFORM_SMALL_WIDTH, PLATFORM_HEIGHT, 0);
	}
}

bool entity_platform_try_collide(Entity* self, Entity* colliding_entity, int x, int y) {
	bool collide = false;

	if(self->data & TILE_PLATFORM_BIT_BIG) {
		collide =
			x >= self->x && x < self->x + PLATFORM_BIG_WIDTH &&
			y >= self->y && y < self->y + PLATFORM_HEIGHT;
	}
	else {
		collide =
			x >= self->x && x < self->x + PLATFORM_SMALL_WIDTH &&
			y >= self->y && y < self->y + PLATFORM_HEIGHT;
	}

	if(collide && colliding_entity->type == ENTITY_TYPE_PLAYER && self->data & TILE_PLATFORM_BIT_FALL) {
		/* Only fall if player is coming from above */
		if(colliding_entity->y < y) {
			if(get_fall_frames(self) == 0) {
				set_fall_frames(self, 60);
			}
		}
	}

	return collide;
}