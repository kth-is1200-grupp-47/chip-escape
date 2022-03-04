/*
 * This file contains functions for interacting with the ChipKIT EEPROM.
 * Written by Botan Botani
 */

#include <pic32mx.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef EEPROM_H
#define EEPROM_H

void init_i2c();
void transfer_i2c(uint8_t byte, int index);
int receive_i2c(int index);

void i2c_idle();
bool i2c_send(uint8_t data);
uint8_t i2c_recv();
void i2c_ack();
void i2c_nack();
void i2c_start();
void i2c_restart();
void i2c_stop();



#endif