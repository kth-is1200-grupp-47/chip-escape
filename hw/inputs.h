/* Written by Botan Botani */

#ifndef INPUTS_H
#define INPUTS_H

#include<stdint.h>
#include <pic32mx.h>
#include <stdbool.h>

int getSW();
int getBtns();

void input_init();
/* Runs at beginning of every frame */
void input_poll();

bool input_get_button(int button);
bool input_get_button_pressed(int button);
bool input_get_button_released(int button);

#endif