#include <stdint.h>
#include <pic32mx.h>
#include "hw/timer.h"
#include "hw/interrupts.h"

/* Total milliseconds elasped by program */
volatile int milliseconds = 0;

/*****************************************
 * This function initializes the timers. *
 * Timer 2 and 3 are combined to form a  *
 * 32-bit timer.                         *
 * TMR2 = LSWord, TMR3 MSWord            *
 * Written by Botan Botani               *
 ****************************************/
void timer_init() {

    /* Clear timer 2 and 3 */
    T2CON = 0;
    TMR2 = 0;
    T3CON = 0;
    TMR3 = 0;

    /* Set the period to 2^32 - 1 */
    PR2 = 0xFFFFFFFF;

    /************************************
     * Bit 15 enables the timer         *
     * Bit 6-4 sets the prescaler(1:256)*
     * Bit 3 enables 32 bit mode        *
     ***********************************/
    T2CONSET = 0x8078;

    /* Clear timer 4 */
    T4CON = 0;
    TMR4 = 0;

    /**************************************
     * TMR4 counts to 1250 which is 1 ms *
     * (80 000 000 / 64) / 1000 = 1250    *
     *************************************/
    PR4 = 1250;

    /************************************
     * Bit 15 enables the timer         *
     * Bit 6-4 sets the prescaler(1:256)*
     ***********************************/
    T4CONSET = 0x8060;

}


/**************************************************
 * This function returns the time in milliseconds *
 * Written by Botan Botani                        *
 *************************************************/
int timer_time(){
    return milliseconds;
}

/******************************************
 * This function waits for x milliseconds *
 * Written by Botan Botani                *
 *****************************************/

void timer_wait(int ms){
    /* Current time */
    volatile int temp_ms = milliseconds;

    while((milliseconds - temp_ms) < ms){
        /* wait */
    }
}

