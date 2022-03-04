#include "game/entity.h"
#include "game/camera.h"
#include "game/entity/player.h"
#include "game/difficulty.h"

#include "debug.h"
#include "hw/display.h"

void get_bit_position(Entity* self, uint8_t bit, uint8_t* x, uint8_t* y) {
	*x = (self->data >> (bit * 8)) & 0xf;
	*y = (self->data >> (bit * 8 + 4)) & 0xf;
}

void set_bit_position(Entity* self, uint8_t bit, uint8_t x, uint8_t y) {
	/* Clear X */
	self->data &= ~(0x0000000f << (bit * 8));
	self->data |= x << (bit * 8);
	/* Clear Y */
	self->data &= ~(0x0000000f << (bit * 8 + 4));
	self->data |= y << (bit * 8 + 4);
}

void entity_bits_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata) {
	self->x = tilex * TILE_SIZE;
	self->y = tiley * TILE_SIZE;

	/* How many bits to spawn? */
	switch(level_extract_tile_data(tiledata))
	{
		/* Spawn one bit at center */
		case 1:
			set_bit_position(self, 0, 4, 4);
			break;

		/* Spawn two bits at center */
		case 2:
			set_bit_position(self, 0, 2, 4);
			set_bit_position(self, 1, 6, 4);
			break;

		/* Spawn three bits in triangle */
		case 3:
			set_bit_position(self, 0, 3, 4);
			set_bit_position(self, 1, 5, 4);
			set_bit_position(self, 2, 4, 2);
			break;

		/* Spawn four bits in triangle */
		case 4:
			set_bit_position(self, 0, 2, 2);
			set_bit_position(self, 1, 4, 2);
			set_bit_position(self, 2, 2, 4);
			set_bit_position(self, 3, 4, 4);
			break;

		default:
			crash("Invalid bit count");
	}
}

void entity_bits_update(Entity* self, int framenum) {

}

void entity_bits_draw(Entity* self) {
	for(int i = 0; i < 4; i++) {
		uint8_t bitx, bity;
		get_bit_position(self, i, &bitx, &bity);

		if(bitx || bity) {
			display_write_bit(true, self->x + bitx - camera_offset_x, self->y + bity - camera_offset_y);
		}
	}
}

bool entity_bits_try_collide(Entity* self, Entity* colliding_entity, int x, int y) {
	if(colliding_entity->type == ENTITY_TYPE_PLAYER) {
		for(int i = 0; i < 4; i++) {
			uint8_t bitx, bity;
			get_bit_position(self, i, &bitx, &bity);

			if(bitx || bity) {
				if(x == self->x + bitx && y == self->y + bity) {
					PlayerData* data = (PlayerData*)colliding_entity->data;
					/* Award more points for higher difficulties */
					data->points += 10 + (current_difficulty - DIFFICULTY_EASY) * 5;

					set_bit_position(self, i, 0, 0);
				}
			}
		}
	}

	/* Don't actually collide with player*/
	return false;
}