/*
 * This file contains functions for interacting with the Serial Peripheral Interface.
 * Written by Hannes Mann
 */

#ifndef SPI_H
#define SPI_H

#include <stdint.h>

/* Initialize SPI for display. */
void spi_init();

/* Send and recieve a byte on port 2. */
uint8_t spi2_send(uint8_t data);

#endif