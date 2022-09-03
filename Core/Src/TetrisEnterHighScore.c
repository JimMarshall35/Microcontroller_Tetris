/*
 * TetrisEnterHighScore.c
 *
 *  Created on: Jul 9, 2022
 *      Author: James.Marshall
 */
#include <stdio.h>

#include "TetrisEnterHighScore.h"
#include "TetrisHighScores.h"
#include "main.h"
#include "LcdGraphics.h"
#include "AnalogStick.h"
#include "TetrisPersistantData.h"

#define ENTER_INITIAL_BLINK_PERIOD 500

extern  ADC_HandleTypeDef hadc2;
extern LCD_PCD8544_screen_t gLcdScreen;

static u32 _newHighScore;
static u8 _newHighScoreRank;
static i8 _settingInitial = 0;
static u32 _blinkTimer = 0;
static bool _blinkState = true; /* True if the initial being set is currently drawn to frame buffer, false otherwise */
static bool _isAnalogStickExtended = false; /* Is the analog stick moved far enough in any one direction to count as "extended" */

static void WriteSingleHighScoreToFrameBuffer(const HighScore* highScore, u8 line){
	char highScoreLineBuffer[10];
	sprintf(highScoreLineBuffer,"%c%c%c %lu",
		highScore->Initials[0],
		highScore->Initials[1],
		highScore->Initials[2],
		highScore->Score);
	gfxWriteTextLineToFrameBuffer(line,0,highScoreLineBuffer);

}

static void DrawHighScoresToFrameBuffer(){
	for(u32 i=0; i < NUM_HIGHSCORES_SAVED; i++){
		const HighScore* highScore = TetrisHighScores_GetHighScoreAtIndex(i);
		WriteSingleHighScoreToFrameBuffer(highScore, i+1);
	}

}

static void BlinkInitialBeingSet(){
	if(_blinkState == true){
		_blinkState = false;
		u8 colStart = _settingInitial * LCD_PCD8544_CHAR_WIDTH;
		u8 row = _newHighScoreRank + 1;
		gfxClearFrameBufferRow(row,colStart,LCD_PCD8544_CHAR_WIDTH);
		UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(row,row,0,colStart + LCD_PCD8544_CHAR_WIDTH);
	}
	else{
		_blinkState = true;
		u8 row = _newHighScoreRank + 1;
		const HighScore* highScore = TetrisHighScores_GetHighScoreAtIndex(_newHighScoreRank);
		WriteSingleHighScoreToFrameBuffer(highScore, row);
		UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(row,row,0,LCD_PCD8544_CHAR_WIDTH*3);

	}
	gfxFinishDrawing(&gLcdScreen);
}

Tetris_Modes_StateTriggers TetrisEnterHighScore_Update(u32 timePassed){

	HighScore* highScore;
	switch(EmulateDPad(&hadc2)){
	case UP:
		highScore = TetrisHighScores_GetHighScoreAtIndex(_newHighScoreRank);
		highScore->Initials[_settingInitial]++;
		if(highScore->Initials[_settingInitial] > 'Z'){
			highScore->Initials[_settingInitial] = 'A';
		}
		_blinkState = false;
		_blinkTimer = ENTER_INITIAL_BLINK_PERIOD;
		break;
	case DOWN:
		highScore = TetrisHighScores_GetHighScoreAtIndex(_newHighScoreRank);
		highScore->Initials[_settingInitial]--;
		if(highScore->Initials[_settingInitial] < 'A'){
			highScore->Initials[_settingInitial] = 'Z';
		}
		_blinkState = false;
		_blinkTimer = ENTER_INITIAL_BLINK_PERIOD;
		break;
	case LEFT:
		_settingInitial--;
		if(_settingInitial < 0){
			_settingInitial = 2;
		}
		_blinkState = false;
		_blinkTimer = ENTER_INITIAL_BLINK_PERIOD;
		break;
	case RIGHT:
		_settingInitial++;
		if(_settingInitial >= 3){
			_settingInitial = 0;
		}
		_blinkState = false;
		_blinkTimer = ENTER_INITIAL_BLINK_PERIOD;
		break;
	}

	_blinkTimer += timePassed;
	if(_blinkTimer >= ENTER_INITIAL_BLINK_PERIOD){
		_blinkTimer = 0;
		BlinkInitialBeingSet();
	}

	GPIO_PinState buttonBState = HAL_GPIO_ReadPin(BUTTON_B_GPIO_Port, BUTTON_B_Pin);
	if(buttonBState == GPIO_PIN_SET){
		return HighScoreEntered;
	}

	return NoChange;
}

void TetrisEnterHighScore_OnEnter(void* stateMachineDataPtr, Tetris_Modes_States previousState){
	_newHighScore = *((u32*)stateMachineDataPtr);
	_newHighScoreRank = TetrisHighScores_AddHighScore(_newHighScore);
	gfxClearFrameBuffer();
	ClearUpdateRegions();
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(0,0,0,84);
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(1,1,0,84);
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(2,2,0,84);
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(3,3,0,84);
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(4,4,0,84);
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(5,5,0,84);


	gfxWriteTextLineToFrameBuffer(0,0,"High Score");
	DrawHighScoresToFrameBuffer();
	gfxFinishDrawing(&gLcdScreen);
	_settingInitial = 0;
	_blinkTimer = 0;
	_blinkState = true;

}

void TetrisEnterHighScore_OnExit(void* stateMachineDataPtr, Tetris_Modes_States nextState){
	ClearScreen(&gLcdScreen);
	TetrisPersistantData_SaveAllPersistantData();
}
