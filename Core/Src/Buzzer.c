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

static u16 _notesBuzzerTimerAutoReloadValues[NUM_NOTES] = {
		0,    // off
		72726,
		64791,
		61155,
		54483,
		48539,
		45814,
		40816,
		36363,
		32396,
		30577,
		27241,
		24269,
		22907,
		20407,
		18181,
		16197,
		15288,
		13620,
		12134,
		11453,
		10203,
		9090,
		8098,
		7643,
		6809,
		6066,
		5726,
		5101,
		4544,
		4049,
		3821,
		3404,
		3033,
		2862,
		2550,
		2272,
		2024,
		1910,
		1702,
		1516,
		1431,
		1275,
		1135,
		1011,
		955,
		850,
		757,
		715,
		637,
		567,
		505,
		477

};

void Buzzer_Init(TIM_HandleTypeDef* timer){
	HAL_TIM_OC_Start(timer,TIM_CHANNEL_1);
	_timer = timer;
	__HAL_TIM_SET_AUTORELOAD(_timer,0);
}

void Buzzer_Note(Note note){
	__HAL_TIM_SET_AUTORELOAD(_timer,_notesBuzzerTimerAutoReloadValues[note]);
}
