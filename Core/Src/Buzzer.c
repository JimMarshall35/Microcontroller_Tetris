/*
 * Buzzer.c
 *
 *  Created on: Aug 1, 2022
 *      Author: James.Marshall
 */

#include "main.h"
#include "Buzzer.h"
#include "BasicTypedefs.h"

static TIM_HandleTypeDef* _timer = 0;

static u16 _notesBuzzerTimerAutoReloadValues[8] = {
		0,    // off
		8099, // b
		7644, // c
		6811, // d
		6067, // e
		5727, // f
		5102, // g
		4545, // a
};

void Buzzer_Init(TIM_HandleTypeDef* timer){
	HAL_TIM_OC_Start(timer,TIM_CHANNEL_1);
	_timer = timer;
	__HAL_TIM_SET_AUTORELOAD(_timer,0);
}

void Buzzer_Note(Note note){
	__HAL_TIM_SET_AUTORELOAD(_timer,_notesBuzzerTimerAutoReloadValues[note]);
}
