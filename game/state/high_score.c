#include <stdbool.h>

#include "game/state.h"
#include "hw/inputs.h"

/* Array for initials and scores */
char initials[12];
int scores[4];
int data_array[17];

/* This function converts hex to char */
char hex2char(int hex){
    char ch = (char) hex;
    return ch;
}

/***********************************************************************
 * This function initializes the high score list.                      *
 * It takes the data read from the EEPROM: three initials and a score  *
 * and puts them in an array.                                          *
 ***********************************************************************/
void high_score_load(const int* data){

    /* copy data to an array so that we can save them for later */
    for(int i = 0; i < 17; i++){
        data_array[i] = data[i];
    }

    int count_initials = 0;
    int count_scores = 0;

    /* data begins with a 0 element */
    for(int i = 1; i < 17; i++){
        
        /* Every fourth character is a score */
        if(i%4 == 0){
            scores[count_scores] = data[i];
            count_scores++;
        }

        /* Else it's an initial */
        else{
            initials[count_initials] = hex2char(data[i]);
            count_initials++;
        }
    }
}

void high_score_update(int framenum) {
	int pushed_buttons = getBtns();

	if(pushed_buttons & 0b1000){
		switch_state(STATE_MAIN_MENU, 0);
	}
}

void high_score_draw() {
}

/* Returns true if the score acquired is a high score */
bool is_top_4(int score){

    for(int i = 0; i < 4; i++){
        if(scores[i] < score){
            return true;
        }

        else{
            return false;
        }
    }
}

/************************************************
 * This function adds a high score to the list  *
 * and saves the new high score to the EEPROM.  *
 ************************************************/
void high_score_put_n_save(int data[]){

    for(int i = 4; i < 17; i = i + 4){
        if(data_array[i] < data[3]){
            for(int j = 0; j <= 3; j--){
                data_array[i-j] = data[3-j];
            }

        }
    }

    /* Saves to EEPROM */
    for(int i = 0; i < 17; i++){
        transfer_i2c(data_array[i]);
    }
}
