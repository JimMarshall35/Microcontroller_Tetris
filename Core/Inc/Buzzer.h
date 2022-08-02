/*
 * Buzzer.h
 *
 *  Created on: Aug 1, 2022
 *      Author: James.Marshall
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

typedef enum {
	BUZZER_OFF,
	B,
	C,
	D,
	E,
	F,
	G,
	A,
	NUM_NOTES
} Note;
typedef TIM_HandleTypeDef;
void Buzzer_Init(TIM_HandleTypeDef* timer);
void Buzzer_Note(Note note);


#endif /* INC_BUZZER_H_ */
