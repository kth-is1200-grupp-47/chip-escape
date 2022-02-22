#include<stdint.h>
#include <pic32mx.h>
#include"hw/inputs.h"


/***********************************************
* This file handles various inputs to the game *
************************************************/

/**************************************************** 
*  Input of switches is returned by this function   *
*  in binary form.                                  *
*  Written by Hannes Mann & Botan Botani            *
****************************************************/
int getSW() {
	int sw_status = 0b0000;

    /* Status of switch 1 */
	sw_status |= (PORTD & (1 << 8)) >> 8;

    /* Status of switch 2 */
	sw_status |= (PORTD & (1 << 9)) >> 8;

    /* Status of switch 3 */
	sw_status |= (PORTD & (1 << 10)) >> 8;

    /* Status of switch 4 */
	sw_status |= (PORTD & (1 << 11)) >> 8;

	return sw_status;
}



/**************************************************** 
*  Input of buttons is returned by this function    *
*  in binary form.                                  *
*  Written by Hannes Mann & Botan Botani            *
****************************************************/
int getBtns(){
    int btn_status = 0b0000;

    /* Status for button 1 (jump movement) */
    btn_status |= (PORTF & (1 << 1)) >> 1

    /* Status for button 2 (right movement) */
    btn_status |= (PORTD & (1 << 5)) >> 4;

    /* Status for button 3 */
	btn_status |= (PORTD & (1 << 6)) >> 4;

    /* Status for button 4 (left movement) */
	btn_status |= (PORTD & (1 << 7)) >> 4;

    return btn_status;
}