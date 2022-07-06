/*
 * TetrisGame.c
 *
 *  Created on: Jun 29, 2022
 *      Author: James.Marshall
 *
 * A main loop that calls the functions in TetrisCore.h
 * - an implementation of tetris using TetrisCore
 */
#include <stdlib.h>
#include <stdio.h>

#include "TetrisGame.h"
#include "TetrisCore.h"
#include "AnalogStick.h"
#include "BasicTypedefs.h"
#include "LcdGraphics.h"
#include "main.h"

//#include "stm32f3xx_hal_flash.h"
//__attribute__((__section__(".user_data"))) const char userConfig[64];



extern  ADC_HandleTypeDef hadc2;
extern LCD_PCD8544_screen_t gLcdScreen;
#define TETRIS_BOARD_RIGHT_EDGE_COL 37
#define TETRIS_BOARD_LEFT_EDGE_COL 4
#define TETRIS_SCORE_SINGLE 40
#define TETRIS_SCORE_DOUBLE 100
#define TETRIS_SCORE_TRIPLE 300
#define TETRIS_SCORE_TETRIS 1200
#define TETRIS_LINES_PER_LEVEL_UP 10
#define TETRIS_NUM_LEVELS 21
#define TETRIS_START_LEVEL 5

static u32 _tetrisTimer = 0;
static u32 _autoMoveInterval = 552;
static GPIO_PinState _lastButtonBState = GPIO_PIN_RESET;
static u16 _linesCleared = 0;
static u32 _score = 0;
static bool _newScoreToDisplay = true;
static bool _newLevelToDisplay = true;
static MoveDownResult _movingDownResult = Settled;
static u8 _startLevel;

/* table derived from information from https://tetris.fandom.com/wiki/Tetris_(Game_Boy) */
static const u32 _levelsMoveIntervals[TETRIS_NUM_LEVELS] = {
		887,
		820,
		753,
		686,
		619,
		552,
		469,
		368,
		285,
		184,
		167,
		151,
		134,
		117,
		100,
		100,
		84,
		84,
		67,
		67,
		50
};


/* Current scores for a single a double a triple and a tetris respectively */
static u32 _currentLevelTetrisScores[4];

static u8 _currentLevel = 5;

//u32 FlashAddress = 0x08040000;
static void InitCurrentLevelTetrisScores();

static u8 GetRandomNumberBetweenZeroAndSix(){
	return (u8)rand() % 7;
}


static void DrawStationaryBlock(u8 tlX, u8 tlY, u8 brX, u8 brY){
	gfxDrawAxisAlignedRect(tlX, tlY, brX, brY);
	gfxDrawLine(tlX, tlY, brX, brY);
}

static void WriteLevelToFrameBuffer(){
	u8 levelBuffer[11];
	sprintf(levelBuffer, "Lvl: %lu",_currentLevel);
	gfxWriteTextLineToFrameBuffer(2,TETRIS_BOARD_RIGHT_EDGE_COL + 1,levelBuffer);

}

static void SetLevel(u8 level){
	_currentLevel = level;
	_newLevelToDisplay = true;
	_autoMoveInterval = _levelsMoveIntervals[_currentLevel];
	InitCurrentLevelTetrisScores();
}

static void OnLinesCleared(u8 numLinesCleared){
	_linesCleared += numLinesCleared;
	if(_linesCleared >= TETRIS_LINES_PER_LEVEL_UP){
		_linesCleared = 0;
		SetLevel(_currentLevel + 1);
	}
	switch(numLinesCleared){
	case 1:
		_score += _currentLevelTetrisScores[0];
		break;
	case 2:
		_score += _currentLevelTetrisScores[1];
		break;
	case 3:
		_score += _currentLevelTetrisScores[2];
		break;
	case 4:
		_score += _currentLevelTetrisScores[3];
		break;
	}
	_newScoreToDisplay = true;
}

static void OnGameOver(){
	//_linesCleared = 0;
	//_score = 0;
	//_newScoreToDisplay = true;
	//SetLevel(TETRIS_START_LEVEL);
}

static void IncrementTetrisTimer(u32 timePassed, bool moveDown){
	_tetrisTimer += timePassed;
	if(_tetrisTimer > _autoMoveInterval){
		if(moveDown == true){
			_movingDownResult = Tetris_MoveDown();
		}
		_tetrisTimer = 0;
	}
}

static void WriteScoreToFrameBuffer(){
	u8 scoreNumBuffer[7];
	sprintf(scoreNumBuffer, "%lu",_score);
	gfxWriteTextLineToFrameBuffer(1,TETRIS_BOARD_RIGHT_EDGE_COL + 1,scoreNumBuffer);

}


static void UpdateTetrisGame(u32 timePassed){

	i32 analogXChange, analogYChange;
	ReadAnalogStickChange(&hadc2,&analogXChange,&analogYChange);

	bool movingDown = true;

	GPIO_PinState buttonBState = HAL_GPIO_ReadPin(BUTTON_B_GPIO_Port, BUTTON_B_Pin);

	const i32 twelveBitMax = 4096;
	if(analogXChange > (twelveBitMax/3)){
		Tetris_MoveRight();
	}
	else if(analogXChange < -(twelveBitMax/3)){
		Tetris_MoveLeft();
	}
	else if(analogYChange > (twelveBitMax/3)){
		_movingDownResult = Tetris_MoveDown();
		movingDown = false;
	}
	if(buttonBState == GPIO_PIN_SET && _lastButtonBState == GPIO_PIN_RESET){
		Tetris_Rotate();
	}

	_lastButtonBState = buttonBState;

	IncrementTetrisTimer(timePassed, movingDown);

}

static void DrawTetrisGame(){
	gfxClearFrameBuffer();

	Tetris_DrawTetrisBoard((_movingDownResult == Settled || _movingDownResult == GameOver) ? true : false);

	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(0,5,TETRIS_BOARD_LEFT_EDGE_COL,TETRIS_BOARD_RIGHT_EDGE_COL);

	if(_movingDownResult == Settled || _movingDownResult == GameOver){
		/* update the area where the next block is shown */
		UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(0,0,0,53);
	}
	if(_newScoreToDisplay == true){
		/* Update the new area of the screen with the new score.
		 * TODO: get actual length of score string instead of using 84 */
		WriteScoreToFrameBuffer();
		UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(1,1,0,84);
		_newScoreToDisplay = false;
	}
	if(_newLevelToDisplay){
		WriteLevelToFrameBuffer();
		UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(2,2,0,84);
		_newLevelToDisplay = false;
	}
	gfxFinishDrawing(&gLcdScreen);
}

static void InitCurrentLevelTetrisScores(){
	/* score setting function derived from information from https://tetris.fandom.com/wiki/Scoring */

	if(_currentLevel == 0){
		_currentLevelTetrisScores[0] = 40;
		_currentLevelTetrisScores[1] = 100;
		_currentLevelTetrisScores[2] = 300;
		_currentLevelTetrisScores[3] = 1200;
	}
	else if(_currentLevel == 1){
		_currentLevelTetrisScores[0] = 80;
		_currentLevelTetrisScores[1] = 200;
		_currentLevelTetrisScores[2] = 600;
		_currentLevelTetrisScores[3] = 2400;

	}
	else if(_currentLevel >=2 && _currentLevel < 9){
		_currentLevelTetrisScores[0] = 120;
		_currentLevelTetrisScores[1] = 300;
		_currentLevelTetrisScores[2] = 900;
		_currentLevelTetrisScores[3] = 360;

	}
	else if(_currentLevel == 9){
		_currentLevelTetrisScores[0] = 400;
		_currentLevelTetrisScores[1] = 1000;
		_currentLevelTetrisScores[2] = 3000;
		_currentLevelTetrisScores[3] = 12000;

	}
	else{
		_currentLevelTetrisScores[0] = 40 * (_currentLevel + 1);
		_currentLevelTetrisScores[1] = 100 * (_currentLevel + 1);
		_currentLevelTetrisScores[2] = 300 * (_currentLevel + 1);
		_currentLevelTetrisScores[3] = 1200 * (_currentLevel + 1);

	}
}


Tetris_Modes_StateTriggers TetrisGame_Update(u32 timePassed){

	UpdateTetrisGame(timePassed);
	DrawTetrisGame();
	if(_movingDownResult == MoveDownResultGameOver){
		TetrisMain_SetStateMachineDataPointer(&_startLevel);
		return GameOver;
	}
	return NoChange;

}



void TetrisGame_Init(){
	Tetris_Init(
			&gfxDrawAxisAlignedRect,
			&DrawStationaryBlock,
			&gfxDrawAxisAlignedRect,
			&GetRandomNumberBetweenZeroAndSix,
			&OnLinesCleared,
			&OnGameOver);
}

void TetrisGame_OnEnter(void* stateMachineDataPtr){
	_startLevel = *((u8*)stateMachineDataPtr);
	SetLevel(_startLevel);
	Tetris_ResetTetrisBoard();
}














