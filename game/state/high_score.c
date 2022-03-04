#include <stdbool.h>
#include<stdio.h>

#include "game/state.h"

#include "hw/display.h"
#include "hw/input.h"

/********************************************
 * Handles high score                       *
 * Written by Botan Botani                  *
 *******************************************/

/* Array for initials and scores */
char initials[4][3];
int scores[4];

/* Count for characters from A -> Z */
int in_count = 0;

/* Count for letters in inputting high score */
int letter_count = 0;

/* Variable for current high score */
char hiscore_initials[] = {'Y', 'O', 'U'};
int hiscore;

/* Variable of where to put high score */
int hiscore_index = -1;

/* Variable for if the game is exiting high score mode */
bool exit = false;


/* This should be run the first time game starts */
void highscore_init(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 3; j++){
            initials[i][j] = 'X';
        }
        scores[i] = 0;
    }
}

/* This function resets variables upon exit */
void highscore_exit(){
    exit = false;
    hiscore_initials[0] = 'Y';
    hiscore_initials[1] = 'O';
    hiscore_initials[2] = 'U';
    hiscore = 0;

    hiscore_index = -1;

    in_count = 0;
    letter_count = 0;
}

/* This function takes inputs from buttons and lets the user change initials to input its high score */
void high_score_update(int framenum) {

    /* This runs if the game is coming from play */
    if(previous_state == STATE_LEVEL){
        if(input_get_btns_pressed() & BUTTON_LEFT){
            if(letter_count == 0){
                letter_count = 25;
                hiscore_initials[in_count] = 0x41 + letter_count;
            }

            else{
                letter_count--;
                hiscore_initials[in_count] = 0x41 + letter_count;
            }
        }

        if(input_get_btns_pressed() & BUTTON_RIGHT){
            if(letter_count == 25){
                letter_count = 0;
                hiscore_initials[in_count] = 0x41 + letter_count;
                }
            else{
                letter_count++;
                hiscore_initials[in_count] = 0x41 + letter_count;
                }
        }

        if(input_get_btns_pressed() & BUTTON_ACTION){
            if(in_count != 2 && previous_state == STATE_LEVEL){
                in_count++;
                letter_count = 0;
            }
            else{
                if(previous_state == STATE_LEVEL){
                    exit = true;
                    high_score_put(hiscore, hiscore_initials);
                }

                highscore_exit();
                switch_state(STATE_MAIN_MENU, 0);
            }

        }
    }


    else{
		if(input_get_btns_pressed() & BUTTON_ACTION) {
            highscore_exit();
            switch_state(STATE_MAIN_MENU, 0);
		}
    }
}

/* This draws the previous high scores */
void line_draw(){
    for(int i = 0; i < 4; i++){
        char scoredraw[20];
        sprintf(scoredraw, "%c%c%c - %d", initials[i][0], initials[i][1], initials[i][2], scores[i]);


        display_draw_text(scoredraw, 5, (i*FONT_CHAR_HEIGHT), 0);
    }
}

/****************************************************
 * This function draws the high score on the screen *
 ****************************************************/
void high_score_draw() {

    /* Checking previous state */
    switch(previous_state){

        case STATE_MAIN_MENU: {
            /* Draws the high scores */
            line_draw();
			break;
		}

        case STATE_LEVEL:{

            line_draw();

            /* Array for printing input */
            char inputdraw[20];

            sprintf(inputdraw, "%c%c%c - %d", hiscore_initials[0], hiscore_initials[1], hiscore_initials[2], hiscore);

            display_draw_text(inputdraw, 60, (3*FONT_CHAR_HEIGHT), 0);
			break;

        }
    }



}

/* Returns true if the score acquired is a high score */
bool is_top_4(int score){

    for(int i = 0; i < 4; i++){
        if(scores[i] < score){
            hiscore_index = i;
            return true;
        }
    }

	return false;
}

/* This function puts the high score where it should be */
void high_score_put(int data, char inits[]){
    int temp_score;
    char temp_inits[3];

    int temp_score2;
    char temp_inits2[3];

    /* Checks if we're in exit mode */
    if(!exit){

        /* Puts the high score where it should be and moves down the rest */
        for(int i = 0; i < 4; i++){
            if(i == hiscore_index){
                for(int k = 0; k < 3; k++){
                    temp_inits[k] = initials[i][k];
                    initials[i][k] = inits[k];
                }
                temp_score = scores[i];
                scores[i] = data;



            }
            else if(i > hiscore_index){
                for(int k = 0; k < 3; k++){
                    temp_inits2[k] = initials[i][k];
                }
                temp_score2 = scores[i];

                for(int k = 0; k < 3; k++){
                    initials[i][k] = temp_inits[k];
                }
                scores[i] = temp_score;

                for(int k = 0; k < 3; k++){
                    temp_inits[k] = temp_inits2[k];
                }
                temp_score = temp_score2;
            }
        }
    }

    else if(exit){
        for(int k = 0; k < 3; k++){
			initials[hiscore_index][k] = inits[k];
		}
		scores[hiscore_index] = data;
    }
}

/* This loads the high score */
void high_score_load(int data){
    hiscore = data;

    if((previous_state == STATE_LEVEL) & (data != 0)){
        bool is_high_score = is_top_4(data);

        if(is_high_score){
            /* Temp variable for acquired high score */
            hiscore = data;

            /* Saves the temp high score and displays it */
            high_score_put(hiscore, hiscore_initials);
        }

        else{
            previous_state = STATE_MAIN_MENU;
        }
    }

}
