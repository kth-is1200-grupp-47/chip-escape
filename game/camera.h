/*
 * The camera follows the player around during the game.
 * Written by Hannes Mann
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "game/level.h"
#include "game/entity.h"

/* Offset calculated in camera_update */
extern int camera_offset_x;
extern int camera_offset_y;

/* Reset the camera and focus on new player entity */
void camera_reset(Entity* player);

/* Run after updating entities */
void camera_update(int framenum);

#endif