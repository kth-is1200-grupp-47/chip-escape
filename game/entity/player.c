#include "game/camera.h"
#include "game/entity.h"
#include "game/difficulty.h"

#include "hw/display.h"
#include "hw/input.h"

USE_IMAGE(player);
USE_IMAGE(icons);

#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 1

/* 0.12 */
#define GRAVITY 12

typedef struct {
	/* Speed in both directions. Divide by 100 to get pixel value */
	int speed_x, speed_y, speed_rx, speed_ry;

	/* Direction player is facing. */
	uint8_t direction;
	/* If player is standing on the ground */
	bool on_ground;

	/* Walk animation frames */
	uint8_t animation_frame;
	/* Frames since player started jumping */
	uint8_t jump_frames;

	/* Amount of lives left */
	uint8_t lives_left;
	/* Amount of points player has collected */
	uint32_t points;
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
	pdata.lives_left = difficulty_life_count[current_difficulty];

	self->data = (uint32_t)&pdata;
}

void entity_player_update(Entity* self, int framenum) {
	PlayerData* data = (PlayerData*)self->data;

	if(input_get_btns() & BUTTON_LEFT) {
		data->speed_x = -100;
		data->direction = DIRECTION_LEFT;
		data->animation_frame++;
	}
	else if(input_get_btns() & BUTTON_RIGHT) {
		data->speed_x = 100;
		data->direction = DIRECTION_RIGHT;
		data->animation_frame++;
	}
	else {
		data->animation_frame = 0;

		/* Reduce speed slowly. Max speed must be divisible by this value. */
		if(data->speed_x > 0) {
			data->speed_x -= 20;
		}
		else if(data->speed_x < 0) {
			data->speed_x += 20;
		}
	}

	/* Wrap around animation after 40 frames */
	data->animation_frame = data->animation_frame % 40;

	/* The player can hold the jump button for a few frames to select jump height */
	bool can_hold_jump =
		/* Condition A: Player is on ground AND just pressed the jump button */
		(data->on_ground && input_get_btns_pressed() & BUTTON_JUMP) ||
		/* Condition B: Player just pressed the jump button AND keeps holding */
		(data->jump_frames > 0 && input_get_btns() & BUTTON_JUMP);

	if(can_hold_jump) {
		data->speed_y = -160;
		data->jump_frames++;

		/* Player can no longer hold jump */
		if(data->jump_frames > 5) {
			data->jump_frames = 0;
		}
	}
	else {
		data->jump_frames = 0;
	}

	/* Always apply constant gravity */
	data->speed_y += GRAVITY;
	/* Air resistance */
	if(data->speed_y > 200) {
		data->speed_y = 200;
	}

	entity_move(self, &data->speed_x, &data->speed_y, &data->speed_rx, &data->speed_ry, true);

	/* Check if we're on the ground after moving */
	for(int x = self->x; x < self->x + ENTITY_PLAYER_WIDTH; x++) {
		if(entity_try_collide_all(x, self->y + ENTITY_PLAYER_HEIGHT)) {
			data->on_ground = true;
			return;
		}
	}

	/* If loop ended we're in the air */
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

	/* HUD */
	display_draw_image_region(image_icons, 0, 1, 0, 0, 7, 6, OP_OVERLAY);
	char lives[] = { '0' + data->lives_left, '\0' };
	display_draw_text(lives, 8, 0, OP_OVERLAY);

	char buffer[16];
	sprintf(buffer, "%uP", data->points);
	display_draw_text(buffer, 20, 0, OP_OVERLAY);
}