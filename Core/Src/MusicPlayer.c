/*
 * MusicPlayer.c
 *
 *  Created on: Aug 1, 2022
 *      Author: James.Marshall
 */

#include "MusicPlayer.h"

#define MUSIC_SEQUENCER_INTERVAL_MILLISECONDS 10



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
	if(_musicPlayerState != STOPPED){
		return;
	}
	_currentTune = notes;
	_currentTuneNumNotes = numNotes;
	_musicPlayerState = PLAYING;
	_thisNoteTimer = 0;
	_tuneTimer = 0;
	_thisNoteStarted = false;
}

void MusicPlayer_TimerISR(){
	if(_musicPlayerState != PLAYING){
		return;
	}
	_tuneTimer += MUSIC_SEQUENCER_INTERVAL_MILLISECONDS;



	if(_thisNoteStarted == false){
		Buzzer_Note(_currentTune[_currentTunePosition++].note);
		if(_currentTunePosition == _currentTuneNumNotes){
			_currentTunePosition = 0;
		}
		_thisNoteStarted = true;
	}
	else{
		u32 duration = _currentTunePosition == 0 ?
				_currentTune[_currentTuneNumNotes - 1].duration :
				_currentTune[_currentTunePosition - 1].duration;
		if(_thisNoteTimer > duration){
			_thisNoteStarted = false;
			_thisNoteTimer = 0;
		}
		_thisNoteTimer += MUSIC_SEQUENCER_INTERVAL_MILLISECONDS;
	}

}

void MusicPlayer_StopTune(){
	if(_musicPlayerState == PLAYING){
		_musicPlayerState = STOPPED;
	}
}

void MusicPlayer_PauseTune(){
	if(_musicPlayerState == PLAYING){
		_musicPlayerState = PAUSED;
		Buzzer_Note(0);
	}
}

void MusicPlayer_UnPauseTune(){
	if(_musicPlayerState == PAUSED){
		_musicPlayerState = PLAYING;
	}
}

PlayerState MusicPlayer_GetState(){
	return _musicPlayerState;
}
