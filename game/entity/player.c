#include <assert.h>

#include "game/camera.h"
#include "game/entity.h"
#include "game/difficulty.h"
#include "game/state.h"
#include "game/entity/player.h"

#include "hw/display.h"
#include "hw/input.h"
#include "hw/timer.h"

/* From level.c */
extern Level current_level;

USE_IMAGE(player);
USE_IMAGE(icons);

/* 0.12 */
#define GRAVITY 12

/* Only one player can exist at a time, so we don't need to allocate more */
PlayerData pdata;

void entity_player_spawn(Entity* self, int tilex, int tiley, LevelTile tiledata) {
	/* Spawn player on top of tile */
	self->x = (tilex * TILE_SIZE) + TILE_SIZE / 2 - ENTITY_PLAYER_WIDTH / 2;
	self->y = (tiley * TILE_SIZE) - ENTITY_PLAYER_HEIGHT;

	/* Initialize new player data */
	pdata.direction = DIRECTION_RIGHT;

	pdata.speed_x = 0;
	pdata.speed_y = 0;
	pdata.speed_rx = 0;
	pdata.speed_ry = 0;

	pdata.on_ground = true;
	pdata.animation_frame = 0;
	pdata.jump_frames = 0;

	if(!pdata.coming_from_previous_level) {
		pdata.points = 0;
		pdata.lives_left = difficulty_life_count[current_difficulty];
	}

	pdata.coming_from_previous_level = false;
	self->data = (uint32_t)&pdata;
}

void entity_player_update(Entity* self, int framenum) {
	PlayerData* data = (PlayerData*)self->data;

	/* Stop player from moving for first half second when switching states */
	if(framenum > 30) {
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

	/* Check collision around player when standing still so enemies can still kill */
	if(data->speed_x > -1) {
		for(int y = self->y; y < self->y + ENTITY_PLAYER_HEIGHT; y++) {
			entity_try_collide_all(self, self->x, y);
		}
	}
	if(data->speed_x < 1) {
		for(int y = self->y; y < self->y + ENTITY_PLAYER_HEIGHT; y++) {
			entity_try_collide_all(self, self->x + ENTITY_PLAYER_WIDTH - 1, y);
		}
	}

	entity_move(self, &data->speed_x, &data->speed_y, &data->speed_rx, &data->speed_ry, true);

	/* Check if we're on the ground after moving */
	for(int x = self->x; x < self->x + ENTITY_PLAYER_WIDTH; x++) {
		if(entity_try_collide_all(self, x, self->y + ENTITY_PLAYER_HEIGHT)) {
			data->on_ground = true;
			return;
		}
	}

	/* If loop ended we're in the air */
	data->on_ground = false;

	/* If we're below the map we've died */
	if(self->y >= level_h(current_level) * TILE_SIZE - ENTITY_PLAYER_HEIGHT / 2) {
		entity_kill(self);
	}
}

void draw_self(Entity* self, DisplayOp effect) {
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
	display_draw_image_region(image_player, self->x - camera_offset_x, self->y - camera_offset_y, rx, ry, 8, 11, effect);
}

void draw_hud(Entity* self, DisplayOp effect) {
	PlayerData* data = (PlayerData*)self->data;

	display_draw_image_region(image_icons, 0, 1, 0, 0, 7, 6, effect);

	/* Draw life count (max 99) */
	char lives[3];
	sprintf(lives, "%d", data->lives_left);
	display_draw_text(lives, 8, 0, effect);

	char buffer[16];
	sprintf(buffer, "%uP", data->points);
	display_draw_text(buffer, 20, 0, effect);
}

void entity_player_draw(Entity* self) {
	draw_self(self, OP_OVERLAY);
	draw_hud(self, OP_OVERLAY);
}

void entity_player_kill(Entity* self) {
	PlayerData* data = (PlayerData*)self->data;

	data->coming_from_previous_level = true;
	bool can_revive = data->lives_left-- > 1;
	/* Lose 5% pts when dying */
	data->points -= data->points / 20;

	/* Revive self */
	self->type = ENTITY_TYPE_PLAYER;

	/* Draw death animation (white background with player) */
	display_clear(true);
	display_send_buffer();
	timer_wait(320);

	/* Show where the player died */
	draw_self(self, OP_DISABLE);
	display_send_buffer();
	timer_wait(1600);

	/* Show button prompt */
	const char* text = can_revive ? "[BTN4] Revive" : "[BTN4] Highscores";
	display_clear(true);
	display_draw_text(text, DISPLAY_WIDTH / 2 - strlen(text) * FONT_CHAR_WIDTH / 2, DISPLAY_HEIGHT / 2 - FONT_CHAR_HEIGHT / 2, OP_DISABLE);
	display_send_buffer();

	/* Wait until player revives */
	while(~input_get_btns_pressed() & BUTTON_ACTION) {
		input_poll();
		timer_wait(10);
	}

	/* If the player can't revive we save their points in the highscore table */
	if(!can_revive) {
		switch_state(STATE_HIGHSCORE_LIST, (const void*)data->points);
		pdata.coming_from_previous_level = false;
		return;
	}

	/* Respawn the player */
	for(int y = 0; y != level_h(current_level); ++y) {
		for(int x = 0; x != level_w(current_level); ++x) {
			LevelTile tile = current_level[(y * level_w(current_level) + x)];

			if(level_extract_tile_id(tile) == TILE_ID_PLAYER) {
				entity_player_spawn(self, x, y, level_extract_tile_data(tile));
				break;
			}
		}
	}
}

USE_LEVEL(level1);
USE_LEVEL(level2);
USE_LEVEL(level3);

void entity_player_touched_flag(Entity* self) {
	pdata.coming_from_previous_level = true;
	/* Give player points for completing level */
	pdata.points += 1000 + 500 * (current_difficulty - DIFFICULTY_EASY);

	Level next_level = NULL;
	bool won_game = false;

	if(current_level == level_level1) {
		next_level = level_level2;
	}
	else if(current_level == level_level2) {
		next_level = level_level3;
	}
	else if(current_level == level_level3) {
		won_game = true;
	}

	assert(next_level != NULL || won_game);

	if(won_game) {
		switch_state(STATE_HIGHSCORE_LIST, (const void*)pdata.points);
	}
	else {
		switch_state(STATE_LEVEL, next_level);
	}
}