/*
 * MusicPlayer.c
 *
 *  Created on: Aug 1, 2022
 *      Author: James.Marshall
 */

#include "MusicPlayer.h"

#define MUSIC_SEQUENCER_INTERVAL_MILLISECONDS 10

typedef enum{
	STOPPED,
	PLAYING,
	PAUSED
}PlayerState;

static TIM_HandleTypeDef* _timer;

static MusicNote* _currentTune;

static u32 _currentTuneNumNotes = 0;

static u32 _currentTunePosition = 0;

static PlayerState _musicPlayerState = STOPPED;

static u32 _tuneTimer = 0;

static u32 _thisNoteTimer = 0;

static bool _thisNoteStarted = false;

void MusicPlayer_Init(TIM_HandleTypeDef* timer){

}

void MusicPlayer_StartTune(MusicNote* notes, u32 numNotes){
	_currentTune = notes;
	_currentTuneNumNotes = numNotes;
	_musicPlayerState = PLAYING;
	_thisNoteTimer = 0;
	_tuneTimer = 0;
	_thisNoteStarted = false;
}

void MusicPlayer_TimerISR(){
	_tuneTimer += MUSIC_SEQUENCER_INTERVAL_MILLISECONDS;
	if(_musicPlayerState != PLAYING){
		return;
	}


	if(_thisNoteStarted == false){
		Buzzer_Note(_currentTune[_currentTunePosition++].note);
		if(_currentTunePosition == _currentTuneNumNotes){
			_currentTunePosition = 0;
		}
		_thisNoteStarted = true;
	}
	else{
		u32 duration = _currentTunePosition == 0 ? _currentTune[_currentTuneNumNotes - 1].duration : _currentTune
				[_currentTunePosition - 1].duration;
		if(_thisNoteTimer > duration){
			_thisNoteStarted = false;
			_thisNoteTimer = 0;
		}
		_thisNoteTimer += MUSIC_SEQUENCER_INTERVAL_MILLISECONDS;
	}

}
