#include <stdbool.h>
#include<stdio.h>

#include "game/state.h"

#include "hw/display.h"
#include "hw/eeprom.h"
#include "hw/input.h"

/* Array for initials and scores */
char initials[13];
int scores[4];

int data_array[] = {0, 0x41, 0x42, 0x4b, 50, 0x43, 0x44, 0x4c, 40, 0x45, 0x46, 0x4d, 30, 0x47, 0x48, 0x4e, 3};
//int data_array[17];

/* Count for characters from A -> Z */
int in_count = 0;

/* Count for letters in inputting high score */
int letter_count = 0;

/* Variable for current high score */
int hiscore[] = {0x41, 0x41, 0x41, 0};

/* Variable of where to put high score */
int hiscore_index = -1;

/* This function converts hex to char */
char hex2char(int hex){
    char ch = (char) hex;
    return ch;
}

void high_score_put_n_save(int data[]);

void high_score_load(int data){

    hiscore[3] = data;

    if(previous_state == STATE_LEVEL){
        /* Temp variable for acquired high score */
        int temp[] = {0x59, 0x4f, 0x55, hiscore[3]};

        /* Saves the temp high score and displays it */
        high_score_put_n_save(temp);
    }

    int count_initials = 1;
    int count_scores = 0;

    /* This section adds score and initials to separate arrays */
    /* data begins with a 0 element */
    for(int i = 1; i < 17; i++){

        /* Every fourth character is a score */
        if(i%4 == 0){
            scores[count_scores] = data_array[i];
            count_scores++;
        }

        /* Else it's an initial */
        else{
            initials[count_initials] = hex2char(data_array[i]);
            count_initials++;
        }
    }


}

void high_score_update(int framenum) {
    if(input_get_btns_pressed() & BUTTON_LEFT){
        if(letter_count == 0){
            letter_count = 25;
            hiscore[in_count] = 0x41 + letter_count;
        }

        else{
            letter_count--;
            hiscore[in_count] = 0x41 + letter_count;
        }
    }

    if(input_get_btns_pressed() & BUTTON_RIGHT){
        if(letter_count == 25){
            letter_count = 0;
            hiscore[in_count] = 0x41 + letter_count;
            }
        else{
            letter_count++;
            hiscore[in_count] = 0x41 + letter_count;
            }
    }

    if(input_get_btns_pressed() & BUTTON_ACTION){
        if(in_count != 2 && previous_state == STATE_LEVEL){
            in_count++;
            letter_count = 0;
        }
        else{
			if(previous_state == STATE_LEVEL)
            	high_score_put_n_save(hiscore);
            switch_state(STATE_MAIN_MENU, 0);
        }

    }
}

void line_draw(){

    /* Initial count */
    int ic = 1;

    for(int i = 0; i < 4; i++){
        char scoredraw[20];
        sprintf(scoredraw, "%c%c%c - %d", initials[ic], initials[ic+1], initials[ic+2], scores[i]);

        display_draw_text(scoredraw, 5, (i*FONT_CHAR_HEIGHT), 0);
        ic += 2;
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
            char inputdraw[10];

            sprintf(inputdraw, "%c%c%c - %d", hex2char(hiscore[0]), hex2char(hiscore[1]), hex2char(hiscore[2]), hiscore[3]);

            display_draw_text(inputdraw, 60, (3*FONT_CHAR_HEIGHT), 0);
			break;

        }
    }



}

/* Returns true if the score acquired is a high score */
bool is_top_4(int score){

    for(int i = 0; i < 4; i++){
        if(scores[i] < score){
            return true;
        }
    }

	return false;
}

/************************************************
 * This function adds a high score to the list  *
 * and saves the new high score to the EEPROM.  *
 ************************************************/
void high_score_put_n_save(int data[]){
    int temp_array[17];

    if(hiscore_index == -1){
        /* Finds high score index */
        for(int i = 4; i < 17; i = i + 4){
                    hiscore_index = i;
                    break;
                }

        /* Put it where it should be */
        for(int i = 4; i < 17; i = i +4){
            if(i < hiscore_index){
                for(int j = 0; j <= 3; j++){
                    temp_array[i-j] = data_array[i-j];
                }
            }
            else if(i == hiscore_index){
                for(int j = 0; j <= 3; j++){
                    temp_array[i-j] = data[3-j];
                }
            }

            else if(i > hiscore_index){
                for(int j = 0; j <= 3; j++){
                    temp_array[i-j] = data_array[(i-4)-j];
                }
            }
        }

        for(int i = 0; i < 17; i++){
            data_array[i] = temp_array[i];
        }
        }

        else{
            for(int j = 0; j <= 3; j++){
                    data_array[hiscore_index-j] = data[3-j];
                }
        }


}

