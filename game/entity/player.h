#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

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

	/* If player is continuing from previous level */
	bool coming_from_previous_level;
} PlayerData;

#endif