#include "game/level.h"
#include "game/entity.h"
#include "game/camera.h"

#include "hw/display.h"

#define PLATFORM_SMALL_WIDTH 13
#define PLATFORM_BIG_WIDTH 23
#define PLATFORM_HEIGHT 4

USE_IMAGE(tile_platform);

void entity_platform_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata) {
	uint8_t bits = level_extract_tile_data(tiledata);
	/* Reinterpret data pointer as integer */
	uint32_t* data_as_int = (uint32_t*)(self->data);
	*data_as_int = bits;

	/* Spawn platform in center of tile */
	self->x = (tilex * TILE_SIZE) + TILE_SIZE /* center of two tiles */ - PLATFORM_SMALL_WIDTH / 2;
	self->y = (tiley * TILE_SIZE) + TILE_SIZE / 2 - PLATFORM_HEIGHT / 2;

	if(bits & TILE_PLATFORM_BIT_BIG) {
		self->x = (tilex * TILE_SIZE) + (TILE_SIZE * 2) / 3 /* center of three tiles */ - PLATFORM_BIG_WIDTH / 2;
	}
}

void entity_platform_update(Entity* self) {

}

void entity_platform_draw(Entity* self) {
	uint32_t* data_as_int = (uint32_t*)(self->data);

	if(*data_as_int & TILE_PLATFORM_BIT_BIG) {
		display_draw_image_region(image_tile_platform, self->x - camera_offset_x, self->y - camera_offset_y, 0, PLATFORM_HEIGHT, PLATFORM_BIG_WIDTH, PLATFORM_HEIGHT, 0);
	}
	else {
		display_draw_image_region(image_tile_platform, self->x - camera_offset_x, self->y - camera_offset_y, 0, 0, PLATFORM_SMALL_WIDTH, PLATFORM_HEIGHT, 0);
	}
}