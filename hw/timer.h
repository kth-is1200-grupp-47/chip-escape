/*
 * This file contains functions for interacting with the ChipKIT timer.
 * Written by Botan Botani
 */

#include <pic32mx.h>

#ifndef TIMER_H
#define TIMER_H

void timer_init();
int timer_time();
void timer_wait(int x);
extern volatile int milliseconds;

#endif