#include <stdint.h>
#include <pic32mx.h>
#include "hw/timer.h"

/*******************************************************************
 * This file handles the various interrupts                        *
 ******************************************************************/

/*************************************************
 * This function initializes the interrupts used *
 * Written by Botan Botani                       *
 ************************************************/
void init_interrupt(){
    
    /* Clear Interrupt Service Flag for Timer 4 (bit 0) */
    IFSCLR(0) = 0x1;

    /****************************************************
     * Set priority and sub-priority for Timer 4 (7, 3) *
     * Bits 4-2 represent priority and 1-0 sub-priority *
     ****************************************************/
    
    IPCSET(4) = 0x1f;

    /****************************************************
     * Enable interrupt for Timer 4 (bit 0)             *
     ***************************************************/

    IEC(0) = 0x1;

    /**********************************************************
     * To-do: enable interrupts by executing 'ei' instruction *
     *********************************************************/
    // enable_interrupt()
}

/************************************************
 * Interrupt Service Routine                    *
 ***********************************************/
void user_isr(){
    
    /* If interrupt came from timer 4 */
    if (IFS(0) & 0x1){
        /* Add one millisecond */

        /* TODO: create variable for elasped milliseconds
        milliseconds++; */

        /* Clear interrupt flag for timer 4 (bit 0) */
        IFSCLR(0) = 0x1;
    }
}