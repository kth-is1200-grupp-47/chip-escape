#include "game/camera.h"
#include "game/entity.h"

#include "hw/display.h"
#include "hw/inputs.h"

USE_IMAGE(player);

/* From level.c */
extern Level current_level;

#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 1

typedef struct {
	/* Speed in both directions. Divide by 100 to get pixel value */
	int speed_x, speed_rx, speed_y, speed_ry;
	/* Direction player is facing. */
	uint8_t direction;
	/* If player is touching the ground */
	bool on_ground;
} PlayerData;

/* Only one player can exist at a time, so we don't need to allocate more */
PlayerData pdata;

void entity_player_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata) {
	/* Spawn player on top of tile */
	self->x = (tilex * TILE_SIZE) + TILE_SIZE / 2 - ENTITY_PLAYER_WIDTH / 2;
	self->y = (tiley * TILE_SIZE) + TILE_SIZE / 2 - ENTITY_PLAYER_HEIGHT;

	/* Initialize new player data */
	memset(&pdata, 0, sizeof(PlayerData));
	pdata.direction = DIRECTION_RIGHT;
	pdata.on_ground = false;

	self->data = (uint32_t)&pdata;
}

void entity_player_update(Entity* self) {
	PlayerData* data = (PlayerData*)self->data;

	if(getBtns() & 1) {
		data->speed_x = 100;
	}
	else if(getBtns() & 2) {
		data->speed_x = -100;
	}
	else {
		if(data->speed_x > 0) {
			data->speed_x -= 20;
		}
		else if(data->speed_x < 0) {
			data->speed_x += 20;
		}
	}

	data->speed_y += 10; /* TODO gravity collision animation */
	entity_move(self, data->speed_x, data->speed_y, &data->speed_rx, &data->speed_ry);
}

void entity_player_draw(Entity* self) {
	PlayerData* data = (PlayerData*)self->data;

	int rx = 0;
	int ry = data->direction == DIRECTION_LEFT ? 11 : 0;

	/* Player jumping */
	if(!data->on_ground) {
		rx = 24;
	}

	display_draw_image_region(image_player, self->x - camera_offset_x, self->y - camera_offset_y, 0, ry, 8, 11, OP_OVERLAY);
}