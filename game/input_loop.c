
#include<stdint.h>
#include <pic32mx.h>
#include"hw/inputs.h"
#include"game/state.h"
#include"game/state/main_menu.h"
/*************************************************
 * This function checks which buttons are pushed *
 * and depending on which state the game is in   *
 * it performs the necessary op.                 *
 ************************************************/
void loop(){

    /* Variable for pushed buttons */
    int pushed_button = getBtns();

    switch(current_state){
        case STATE_MAIN_MENU:
            
            /************************************************************ 
             * If the game is in the main menu and buttons are pressed  *
             * options are marked temporarily                           * 
             ************************************************************/

            /* High score */
            if(pushed_button & 0b0010){
                current_state = STATE_HIGHSCORE_LIST;
            }

            /* Play */
            if(pushed_buttons & 0b1000){
                current_state = STATE_LEVEL;
            }
        
        /***************************************************************
         * If the game is in LEVEL it moves the player or selects menu *
         **************************************************************/
        case STATE_LEVEL:
            /* JUMP movement */
            if(pushed_buttons & 0b0001){
                /* TODO jump */
            }

            /* RIGHT movement */
            if(pushed_buttons & 0b0010){
                /* TODO RIGHT move */
            }

            /* Exit to MAIN MENU */
            if(pushed_buttons & 0b0100){
                current_state = STATE_MAIN_MENU;
            }

            /* LEFT movement */
            if(pushed_buttons & 0b1000){
                /* TODO Left move */
            }
        
        /* Only option at HIGH_SCORE_LIST is to go back to main menu */
        case STATE_HIGH_SCORE_LIST:
            
            if(pushed_buttons & 0b1000){
                current_state = STATE_MAIN_MENU;
            }

        
    }

}