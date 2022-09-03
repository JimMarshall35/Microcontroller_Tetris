/*
 * MusicPlayer.h
 *
 *  Created on: Aug 1, 2022
 *      Author: James.Marshall
 */

#ifndef INC_MUSICPLAYER_H_
#define INC_MUSICPLAYER_H_
#include "BasicTypedefs.h"
#include "Buzzer.h"

typedef enum{
	STOPPED,
	PLAYING,
	PAUSED
}PlayerState;

typedef struct{
	Note note;
	u32 duration;
} MusicNote;

void MusicPlayer_Init(TIM_HandleTypeDef* timer);

void MusicPlayer_StartTune(MusicNote* notes, u32 numNotes);

void MusicPlayer_StopTune();

void MusicPlayer_PauseTune();

void MusicPlayer_UnPauseTune();

void MusicPlayer_TimerISR();

PlayerState MusicPlayer_GetState();

#endif /* INC_MUSICPLAYER_H_ */
