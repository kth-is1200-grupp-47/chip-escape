
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
    int pushed_buttons = getBtns();

    /* Variable for temporary state */
    GameState temp_state;

    

    switch(current_state){
        case STATE_MAIN_MENU:

            /* Pointer for main_menu_options */
            char *m_ptr = &main_menu_options;

            /* Marks first option */
            temp_state = options2state(*m_ptr);
            
            /************************************************************ 
             * If the game is in the main menu and buttons are pressed  *
             * options are marked temporarily                           * 
             ************************************************************/

            /* Right movement */
            if(pushed_buttons & 0b0010){
                /* Check which option is marked so that it doesn't go past available options */
                if(temp_state != STATE_HIGH_SCORE){
                    ptr++;
                    temp_state = options2state(*m_ptr);
                }
            }

            /* Left movement */
            if(pushed_buttons & 0b1000){
                /* Check which option is marked so that it doesn't go past available options */
                if(temp_state != STATE_LEVEL){
                    ptr--;
                    temp_state = options2state(*m_ptr);
                }
            }
        
        /***************************************************************
         * If the game is in LEVEL it moves the player or selects menu *
         **************************************************************/
        case STATE_LEVEL:
            /* JUMP movement */
            if(pushed_buttons & 0b0001){
                
            }

            /* RIGHT movement */
            if(pushed_buttons & 0b0010){
                
            }

            /* LOAD GAME MENU */
            if(pushed_buttons & 0b0100){
                temp_state = STATE_GAME_MENU;
            }

            /* LEFT movement */
            if(pushed_buttons & 0b1000){
                
            }
        
        /* Only option at HIGH_SCORE_LIST is to go back to main menu */
        case STATE_HIGH_SCORE_LIST:
            
            if(pushed_buttons & 0b1000){
                /* TODO: Back to main menu */
            }

        
    }

        



}