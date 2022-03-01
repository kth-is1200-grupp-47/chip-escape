/* Written by Botan Botani */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include <pic32mx.h>
#include "hw/timer.h"

void init_interrupt();
void user_isr();

#endif