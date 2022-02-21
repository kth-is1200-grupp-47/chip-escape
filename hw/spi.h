/*
 * This file contains functions for interacting with the Serial Peripheral Interface.
 * Written by Hannes Mann
 */

#include <stdint.h>

/* Initialize SPI for display. */
void spi_initialize();

/* Send and recieve a byte on port 2. */
uint8_t spi2_send(uint8_t data);