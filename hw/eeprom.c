#include "hw/eeprom.h"

/********************************************************
 * This file handles I2C bus to transfer data to EEPROM *
 *******************************************************/

/*****************************************************
 * Initializes the I2C1 bus for transfer or read     *
 * Written by Botan Botani                           *
 ****************************************************/
void init_i2c(){

    /* Clear I2C1 bus */

    I2C1CON = 0;
    I2C1STAT = 0;

    /* Testing baud rate */
    I2C1BRG = 0x0C2;

    /* Start I2C1 (bit 15) and stop it in idle mode (bit 13) */
    I2C1CONSET = 0xA000;
}

/*******************************************************
 * This function runs until the I2C bus is ready for   *
 * transfer or receiving.                              *
 * Written by Botan Botani                             *
 ******************************************************/

void ready_i2c(){
    /*********************************************************************************
     * I2CxCON - bit 5 == 0: ACK sent. bit 4 == 0: Acknowledge sequence idle         *
     * bit 3 == 0: Receive sequence not in progress. bit 2 == 0: Stop condition idle *
     * bit 1 == 0: Restart condition idle. bit 0 == 0: start condition idle.         *
     * I2CxSTAT - bit 14: Master transmit is not in progress.                        *
     ********************************************************************************/
    while(I2C1CON & 0x1F || I2C1STAT & 0x4000);
}

/**************************************************************
 * This function transfers 1 byte to the EEPROM with address  *
 * 1010000.                                                   *
 * Parameter: byte                                            *
 * Written by Botan Botani                                    *
 *************************************************************/

void transfer_i2c(uint8_t byte){

    /* Wait for the bus to be ready */
    ready_i2c();

    /* Send start condition (bit 0)*/
    I2C1CONSET = 0x1;

    /* Wait for the bus to be ready */
    ready_i2c();

    /* Device address (7 bits) + Write command (0) */
    I2C1TRN = 10100000;
    if((!(I2C1STAT & 0x8000)){
        /* Send address high byte */
        I2C1TRN = 0x0;

        /* Wait for the bus to be ready */
        ready_i2c();

        /* Send address low byte */
        I2C1TRN = 0x0;

        /* Wait for the bus to be ready */
        ready_i2c();

        /* Store the byte in the i2CxTRN register */
        I2C1TRN = byte;

        /* Wait for the bus to be ready */
        ready_i2c();
    }

    /* Stop the bus */
    I2C1CONSET = 0x4;

}


/**************************************************************
 * This function receives 1 byte from the EEPROM with address *
 * 1010000.                                                   *
 * Parameter: Address                                         *
 * Written by Botan Botani                                    *
 *************************************************************/

uint8_t receive_i2c(uint8_t address){

    /* Wait for the bus to be ready */
    ready_i2c();

    /* Send start condition (bit 0)*/
    I2C1CONSET = 0x1;

    /* Wait for the bus to be ready */
    ready_i2c();

    /* Device address (7 bits) + Write command (0) */
    I2C1TRN = 10100000;
    if((!(I2C1STAT & 0x8000)){
        /* Send address high byte */
        I2C1TRN = 0x0;

        /* Wait for the bus to be ready */
        ready_i2c();

        /* Send address low byte */
        I2C1TRN = 0x0;

        /* Wait for the bus to be ready */
        ready_i2c();

        /* Send start condition (bit 0) and receive enable (bit 3) */
        I2C1CONSET = 0x9;

        /* Wait for the bus to be ready */
        ready_i2c();
    }

    uint8_t received = I2C1RCV;

    /* Stop the bus */
    I2C1CONSET = 0x4;

    return received;
}