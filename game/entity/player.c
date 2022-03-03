#include "game/camera.h"
#include "game/entity.h"

#include "hw/display.h"
#include "hw/inputs.h"

USE_IMAGE(player);

/* From level.c */
extern Level current_level;
extern Entity entities[MAX_ENTITIES + 1];

#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 1

/* 0.12 */
#define GRAVITY 12

typedef struct {
	/* Speed in both directions. Divide by 100 to get pixel value */
	int speed_x, speed_rx, speed_y, speed_ry;

	/* Direction player is facing. */
	uint8_t direction;
	/* If player is standing on the ground */
	bool on_ground;

	/* Walk animation frames */
	uint8_t animation_frame;
	/* Frames since player started jumping */
	uint8_t jump_frames;
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

	self->data = (uint32_t)&pdata;
}

void entity_player_update(Entity* self) {
	PlayerData* data = (PlayerData*)self->data;

	if(getBtns() & 4) {
		data->speed_x = 100;
		data->direction = DIRECTION_RIGHT;
		data->animation_frame++;
	}
	else if(getBtns() & 8) {
		data->speed_x = -100;
		data->direction = DIRECTION_LEFT;
		data->animation_frame++;
	}
	else {
		data->animation_frame = 0;

		if(data->speed_x > 0) {
			data->speed_x -= 20;
		}
		else if(data->speed_x < 0) {
			data->speed_x += 20;
		}
	}

	data->animation_frame = data->animation_frame % 40;

	if((data->on_ground || data->jump_frames > 0) && getBtns() & 2) {
		data->speed_y = -160;
		data->jump_frames++;

		if(data->jump_frames > 5) {
			data->jump_frames = 0;
		}
	}
	else {
		data->jump_frames = 0;
	}

	data->speed_y += GRAVITY;
	/* Air resistance */
	if(data->speed_y > 200) {
		data->speed_y = 200;
	}

	entity_move(self, &data->speed_x, &data->speed_y, &data->speed_rx, &data->speed_ry, true);

	/* Check if we're on the ground after moving */
	for(int x = self->x; x < self->x + ENTITY_PLAYER_WIDTH; x++) {
		if(entity_try_collide_all(entities, x, self->y + ENTITY_PLAYER_HEIGHT)) {
			data->on_ground = true;
			return;
		}
	}
	data->on_ground = false;
}

void entity_player_draw(Entity* self) {
	PlayerData* data = (PlayerData*)self->data;

	int rx = 0;
	if(data->animation_frame / 10 == 1) {
		rx = 8;
	}
	if(data->animation_frame / 10 == 3) {
		rx = 16;
	}
	/* Player jumping */
	if(!data->on_ground) {
		rx = 24;
	}

	int ry = data->direction == DIRECTION_LEFT ? 11 : 0;
	display_draw_image_region(image_player, self->x - camera_offset_x, self->y - camera_offset_y, rx, ry, 8, 11, OP_OVERLAY);
}