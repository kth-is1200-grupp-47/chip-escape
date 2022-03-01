/*
 * This file contains functions for interacting with the ChipKIT EEPROM.
 * Written by Botan Botani
 */

#include <pic32mx.h>
#include <stdint.h>

#ifndef EEPROM_H
#define EEPROM_H

void init_i2c();
void transfer_i2c(uint8_t byte);
uint8_t receive_i2c(uint8_t address);



#endif