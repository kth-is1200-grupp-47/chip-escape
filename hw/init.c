#include <pic32mx.h>

/****************************************
 * This function initializes the inputs *
 * Written by Botan Botani              *
 * *************************************/
void input_init(){
    /* Initialization of buttons 2-4 (bits 5-7 of port D) and switch 1-4 (bits 8-11 of port D) as input*/
    TRISD |= 0xFE0;

    /* Initialization of button 1 (bit 1 of port F) as input */
    TRISF |= 0x2;
}

/*****************************************
 * This function initializes the timers. *
 * Timer 2 and 3 are combined to form a  *
 * 32-bit timer.                         *
 * TMR2 = LSWord, TMR3 MSWord            *
 * Written by Botan Botani               *
 ****************************************/
void timer_init(){

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
}