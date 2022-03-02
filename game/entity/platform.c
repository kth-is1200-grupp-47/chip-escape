#include "game/level.h"
#include "game/entity.h"

void entity_platform_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata) {
	uint8_t bits = level_extract_tile_data(tiledata);
	/* Reinterpret data pointer as integer */
	uint32_t* data_as_integer = (uint32_t*)(self->data);

	if(bits & TILE_PLATFORM_BIT_BIG) {
		*data_as_integer |= 1 << 0;
	}
	if(bits & TILE_PLATFORM_BIT_FALL) {
		*data_as_integer |= 1 << 1;
	}
}

void entity_platform_update(Entity* self) {

}

void entity_platform_draw(Entity* self) {

}